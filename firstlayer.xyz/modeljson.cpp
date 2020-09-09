#include "json.hpp"
#include "libslic3r/Model.hpp"
#include <iostream>

// using namespace Slic3r;
nlohmann::json stl2json(stl_file* stl)
{
    // loose a little precission for the sake of size
    auto r = [](double val) { return std::roundf(val * 10000) / 10000.0; };
    std::vector<double> points;
    for (int i = 0; i < stl->stats.number_of_facets; i++) {
        auto f = &stl->facet_start[i];
        points.push_back(r(f->vertex[0].x));
        points.push_back(r(f->vertex[0].y));
        points.push_back(r(f->vertex[0].z));
        points.push_back(r(f->vertex[1].x));
        points.push_back(r(f->vertex[1].y));
        points.push_back(r(f->vertex[1].z));
        points.push_back(r(f->vertex[2].x));
        points.push_back(r(f->vertex[2].y));
        points.push_back(r(f->vertex[2].z));
    }
    return nlohmann::json { { "vertices", points } };
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    // Force Slic3r STDOUT to STDERR
    int stdout_fd = dup(STDOUT_FILENO);
    dup2(STDERR_FILENO, STDOUT_FILENO);

    try {
        auto model = Model::read_from_file(argv[1]);
        auto mesh = model.raw_mesh();
        mesh.check_topology();
        mesh.repair(); // this calculates number_of_parts
        BoundingBoxf3 bb = mesh.bounding_box();
        auto scalefactor = std::numeric_limits<double>::min();
        for (auto& v : { bb.max.x, bb.max.y, bb.max.z, -bb.max.x, -bb.max.y, -bb.max.z }) {
            scalefactor = std::max(v, scalefactor);
        }
        mesh.scale(1 / scalefactor);
        bb = mesh.bounding_box();
        // std::cout << bb.max.x << " " << bb.min.x << std::endl;
        // std::cout << bb.max.y << " " << bb.min.y << std::endl;
        // std::cout << bb.max.z << " " << bb.min.z << std::endl;

        auto s = stl2json(&mesh.stl).dump();
        write(stdout_fd, s.data(), s.size());

    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
