# firstlayer.xyz
FROM amazonlinux:latest
RUN yum -y update && yum -y upgrade
RUN yum -y install autoconf automake binutils cmake3 gcc gcc-c++ gzip libtool make pkgconfig tar which
RUN ln /usr/bin/cmake3 /usr/bin/cmake

# Can probably speed up this compile by disabling unneeded features
RUN cd /tmp && \
    curl -L https://dl.bintray.com/boostorg/release/1.63.0/source/boost_1_63_0.tar.gz | tar -xz && \
    cd /tmp/boost_1_63_0 && ./bootstrap.sh && ./b2 -j4 --without-python link=static install && \
    cd / && rm -rf /tmp/boost_1_63_0

# If you are seeing "c++: internal compiler error: Killed" here, its because you're out of ram
COPY ./xs /tmp/Slic3r/xs
COPY ./src /tmp/Slic3r/src
RUN mkdir /tmp/Slic3r/build && cd /tmp/Slic3r/build && \
    cmake ../src -DEnable_GUI=0 -DGUI_BUILD_TESTS=0 -DSLIC3R_BUILD_TESTS=0 -DSLIC3R_STATIC=1 -DCMAKE_BUILD_TYPE=Release && \
    cmake --build . -- -j2

COPY ./firstlayer.xyz /tmp/Slic3r/firstlayer.xyz
RUN mkdir /tmp/Slic3r/firstlayer.xyz/build && cd /tmp/Slic3r/firstlayer.xyz/build && \
    cmake .. && make -j4

FROM amazonlinux:latest
COPY --from=0 /tmp/Slic3r/firstlayer.xyz/build/modelinfo /usr/bin
COPY --from=0 /tmp/Slic3r/firstlayer.xyz/build/modeljson /usr/bin
