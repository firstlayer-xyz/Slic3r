# firstlayer.xyz
FROM amazonlinux:latest
RUN yum -y groupinstall "Development Tools" which

RUN cd /tmp && \
    curl -L https://cmake.org/files/v3.18/cmake-3.18.2.tar.gz | tar -xz && \
    cd cmake-3.18.2 && ./bootstrap && make -DCMAKE_USE_OPENSSL=OFF -j4 && make install && \
    cd / && rm -rf /tmp/cmake-3.18.2

RUN cd /tmp && \
    curl -L https://dl.bintray.com/boostorg/release/1.74.0/source/boost_1_74_0.tar.gz | tar -xz && \
    cd /tmp/boost_1_74_0 && ./bootstrap.sh && ./b2 --without-python link=static install && \
    cd / && rm -rf /tmp/boost_1_74_0

RUN mkdir /Slic3r
COPY ./ /Slic3r/
RUN cd /Slic3r/firstlayer.xyz && ./build_slic3r.sh
RUN mkdir -p /Slic3r/firstlayer.xyz/build cd /Slic3r/firstlayer.xyz/build && cmake .. && make -j8

FROM amazonlinux:latest
COPY --from=0 /Slic3r/firstlayer.xyz/build/modelinfo /usr/bin
COPY --from=0 /Slic3r/firstlayer.xyz/build/modeljson /usr/bin
