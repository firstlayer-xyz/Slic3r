#include "json.hpp"
#include "libslic3r/Model.hpp"
#include <iostream>

// using namespace Slic3r;
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

        auto j = nlohmann::json {
            { "number_of_facets", mesh.stl.stats.number_of_facets },
            { "manifold", !!mesh.is_manifold() },
            { "number_of_parts", mesh.stl.stats.number_of_parts },
            { "volume", mesh.volume() },
            {
                "bounding_box",
                {
                    { "min_x", bb.min.x },
                    { "min_y", bb.min.y },
                    { "min_z", bb.min.z },
                    { "max_x", bb.max.x },
                    { "max_y", bb.max.y },
                    { "max_z", bb.max.z },
                },
            },
            {
                "needs_repair",
                {
                    { "degenerate_facets", mesh.stl.stats.degenerate_facets },
                    { "edges_fixed", mesh.stl.stats.edges_fixed },
                    { "facets_removed", mesh.stl.stats.facets_removed },
                    { "facets_added", mesh.stl.stats.facets_added },
                    { "facets_reversed", mesh.stl.stats.facets_reversed },
                    { "backwards_edges", mesh.stl.stats.backwards_edges },
                },
            }
        };

        auto s = j.dump();
        write(stdout_fd, s.data(), s.size());

        // // Generate preview mesh:
        // // mesh.write_binary("test.stl");
        // auto fp = fopen("test2.stlz", "wb+");
        // // Should we colne first?
        // mesh.center_around_origin();
        // bb = mesh.bounding_box();
        // auto max = [](std::vector<double> list) {
        //     auto m = list[0];
        //     for (auto &v : list)
        //     {
        //         m = std::max(m, v);
        //     }
        //     return m;
        // };
        // auto scale = max({bb.max.x, bb.max.y, bb.max.z, -bb.max.x, -bb.max.y, -bb.max.z});
        // mesh.scale(960 / scale);
        // bb = mesh.bounding_box();
        // std::cout << bb.max.x << " " << bb.min.x << std::endl;
        // std::cout << bb.max.y << " " << bb.min.y << std::endl;
        // std::cout << bb.max.z << " " << bb.min.z << std::endl;
        // auto stl = &mesh.stl;
        // for (int i = 0; i < stl->stats.number_of_facets; i++)
        // {
        //     int16_t x1 = stl->facet_start[i].vertex[0].x;
        //     int16_t x2 = stl->facet_start[i].vertex[0].x;
        //     int16_t x3 = stl->facet_start[i].vertex[0].x;
        //     fwrite(&x1, sizeof(x1), 1, fp);
        //     fwrite(&x2, sizeof(x2), 1, fp);
        //     fwrite(&x3, sizeof(x3), 1, fp);
        // }
        // for (int i = 0; i < stl->stats.number_of_facets; i++)
        // {
        //     int16_t y1 = stl->facet_start[i].vertex[0].y;
        //     int16_t y2 = stl->facet_start[i].vertex[0].y;
        //     int16_t y3 = stl->facet_start[i].vertex[0].y;
        //     fwrite(&y1, sizeof(y1), 1, fp);
        //     fwrite(&y2, sizeof(y2), 1, fp);
        //     fwrite(&y3, sizeof(y3), 1, fp);
        // }
        // for (int i = 0; i < stl->stats.number_of_facets; i++)
        // {
        //     int16_t z1 = stl->facet_start[i].vertex[0].z;
        //     int16_t z2 = stl->facet_start[i].vertex[0].z;
        //     int16_t z3 = stl->facet_start[i].vertex[0].z;
        //     fwrite(&z1, sizeof(z1), 1, fp);
        //     fwrite(&z2, sizeof(z2), 1, fp);
        //     fwrite(&z3, sizeof(z3), 1, fp);
        // }
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
