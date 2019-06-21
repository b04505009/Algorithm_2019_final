#include <boost/polygon/polygon.hpp>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>
using namespace boost::polygon;
using namespace boost::polygon::operators;
#define STRINGIFY(x) #x
#define STRINGIFYMACRO(y) STRINGIFY(y)
#define WEST_NORTH 1;
#define EAST_NORTH 2;
#define EAST_SOUTH 3;
#define WEST_SOUTH 4;
typedef point_data<int> point_type;
typedef polygon_90_with_holes_data<int> polygon_type;
typedef polygon_90_set_data<int> polygon_set_type;
void concave_point(polygon_set_type& polygon_set_result);
int main(int argc, char* argv[]) {
    std::ifstream fin(argv[1]);
    // first line
    std::vector<int> op;
    int split_mode;
    std::string tmp;
    fin >> tmp;
    do {
        fin >> tmp;
        switch (tmp[0]) {
            case 'M':
                op.push_back(0);
                break;
            case 'C':
                op.push_back(1);
                break;
            case 'S':
                switch (tmp[1]) {
                    case 'V':
                        op.push_back(2);
                        break;
                    case 'H':
                        op.push_back(3);
                        break;
                    case 'O':
                        op.push_back(4);
                        break;
                }
        }
    } while (tmp[0] != ';');
    /*
    for (int i = 0; i < op.size(); i++) {
        std::cout << op[i];
    }*/
    // std::cout << std::endl;
    std::vector<point_type> data;
    std::queue<polygon_set_type> MERGE_set;
    std::queue<polygon_set_type> CLIPPER_set;

    std::string line;
    std::string tmp_x;
    int tmp_y;
    int mode;
    getline(fin, line);  // nothing
    for (int i = 0; i < op.size() - 1; i++) {
        polygon_set_type polygon_op_set;
        // std::cout << "op:" << op[i] << "\n";
        // std::cout << "i " << i << std::endl;
        getline(fin, line);  // nothing
        /*
        getline(fin, line);  // DATA MERGE CLIPPER
        std::cout << "line first: " << line << std::endl;
        */
        fin >> tmp;  // DATA
        // std::cout << "DATA: " << tmp << std::endl;
        fin >> tmp;  // MERGE or CLIPPER
        // std::cout << "MERGE or CLIPPER: " << tmp << std::endl;
        mode = (tmp == "MERGE") ? 0 : 1;
        getline(fin, line);
        // std::cout << "line: " << line << std::endl;
        fin >> tmp;  // POLYGON
        // std::cout << "tmp: " << tmp << std::endl;
        // std::cout << "tmp " << tmp << std::endl;
        // int j = 0;
        switch (mode) {
            case 0:
                while (tmp == "POLYGON") {
                    fin >> tmp_x;
                    // std::cout << "tmp_x " << tmp_x << std::endl;
                    do {
                        // tmp_x;
                        fin >> tmp_y;
                        // std::cout << "tmp_y " << tmp_y << std::endl;
                        data.push_back(point_type(stoi(tmp_x), tmp_y));
                        fin >> tmp_x;
                        // std::cout << "tmp_x " << tmp_x << std::endl;
                    } while (tmp_x != ";");
                    polygon_type polygon;
                    polygon.set(data.begin(), data.end());
                    /*
                                        std::cout << "winding "
                                                  <<
                       view_as<polygon_90_concept>(polygon).winding()
                                                  << std::endl;
                                        std::cout << "polygons " << std::endl;
                                        */
                    /*
 for (auto it = polygon.begin(); it != polygon.end(); ++it) {
     std::cout << (*it).x() << " " << (*it).y() << "\n";
 }
 */
                    polygon_op_set += polygon;
                    // std::cout << std::endl;
                    data.clear();
                    fin >> tmp;  // POLYGON or END
                    // std::cout << "tmp: " << tmp << std::endl;
                }
                MERGE_set.push(polygon_op_set);
                break;
            case 1:
                while (tmp == "POLYGON") {
                    fin >> tmp_x;
                    // std::cout << "tmp_x " << tmp_x << std::endl;
                    do {
                        // tmp_x;
                        fin >> tmp_y;
                        // std::cout << "tmp_y " << tmp_y << std::endl;
                        data.push_back(point_type(stoi(tmp_x), tmp_y));
                        fin >> tmp_x;
                        // std::cout << "tmp_x " << tmp_x << std::endl;
                    } while (tmp_x != ";");
                    polygon_type polygon;
                    polygon.set(data.begin(), data.end());
                    /*
                                        std::cout << "winding "
                                                  <<
                       view_as<polygon_90_concept>(polygon).winding()
                                                  << std::endl;
                                        std::cout << "polygons " << std::endl;
                                        for (auto it = polygon.begin(); it !=
                       polygon.end(); ++it) { std::cout << (*it).x() << " " <<
                       (*it).y() << "\n";
                                        }
                                        */
                    polygon_op_set += polygon;
                    data.clear();
                    fin >> tmp;  // POLYGON or END
                }
                CLIPPER_set.push(polygon_op_set);
                break;
            default:
                break;
        }

        // std::cout << "j: " << j << std::endl;
        // j = 0;
        getline(fin, line);  // END DATA
        // std::cout << "line end: " << line << std::endl;
    }
    // std::cout << MERGE_set.size() << std::endl;
    // std::cout << CLIPPER_set.size() << std::endl;
    polygon_set_type polygon_set_results;
    std::vector<polygon_type> polygons;
    for (int i = 0; i < op.size() - 1; i++) {
        switch (op[i]) {
            case 0:
                MERGE_set.front().get_polygons(polygons);
                // std::cout << "MERGE_set " << std::endl;
                /*
                for (int i = 0; i < polygons.size(); i++) {
                    for (auto it = polygons[i].begin(); it != polygons[i].end();
                         ++it) {
                        std::cout << (*it).x() << " " << (*it).y() << "\n";
                    }
                }
                */
                polygon_set_results += MERGE_set.front();
                MERGE_set.pop();
                polygons.clear();
                break;

            case 1:
                CLIPPER_set.front().get_polygons(polygons);
                // std::cout << "CLIPPER_set " << std::endl;
                /*
                for (int i = 0; i < polygons.size(); i++) {
                    for (auto it = polygons[i].begin(); it != polygons[i].end();
                         ++it) {
                        std::cout << (*it).x() << " " << (*it).y() << "\n";
                    }
                }
                */
                polygon_set_results -= CLIPPER_set.front();
                CLIPPER_set.pop();
                polygons.clear();
                break;
        }
    }
    // std::vector<polygon_type> polygons;
    polygon_set_results.get(polygons);
    /*
    std::cout << "polygons" << std::endl;
    for (int i = 0; i < polygons.size(); i++) {
        std::cout << "winding "
                  << view_as<polygon_90_concept>(polygons[i]).winding()
                  << std::endl;
        for (auto it = polygons[i].begin(); it != polygons[i].end(); ++it) {
            std::cout << (*it).x() << " " << (*it).y() << "\n";
        }
    }
    std::cout << "area " << area(polygon_set_results) << std::endl;
*/
    std::vector<rectangle_data<int>> rects;
    switch (op.back()) {
        case 2:
            polygon_set_results.get_rectangles(rects, VERTICAL);
            break;
        case 3:
            polygon_set_results.get_rectangles(rects, HORIZONTAL);
            break;
        case 4:
            concave_point(polygon_set_results);
        default:
            break;
    }
    /*
    std::cout << "rectangles\n";
    std::cout << "rects num = " << rects.size() << "\n";
    for (int i = 0; i < rects.size(); i++) {
        std::cout << "i = " << i << ": ";
        std::cout << rects[i].get(WEST) << " ";
        std::cout << rects[i].get(SOUTH) << " ";
        std::cout << rects[i].get(EAST) << " ";
        std::cout << rects[i].get(NORTH) << "\n";
    }
    */
    std::cout << "rects num = " << rects.size() << "\n";
    std::ofstream fout(argv[2]);
    for (int i = 0; i < rects.size(); i++) {
        fout << "RECT " << rects[i].get(WEST) << ' ';
        fout << rects[i].get(SOUTH) << ' ';
        fout << rects[i].get(EAST) << ' ';
        fout << rects[i].get(NORTH) << " ;\n";
    }

    return 0;
}
/*
polygon_result.get_polygons(polygons);
for (int i = 0; i < polygons.size(); i++) {
    for (auto it = polygons[i].begin(); it != polygons[i].end(); ++it) {
        std::cout << (*it).x() << " " << (*it).y() << "\n";
    }
}
*/
bool is_concave(int x1, int x2, int x3, int y1, int y2, int y3) {
    return ((x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2)) < 0 ? true : false;
}

int available_directions(int x1, int x2, int x3, int y1, int y2, int y3) {
    if (x1 == x2) {
        if (y1 < y2) {
            if (x3 > x2) {
                return WEST_NORTH;
            } else {
                return EAST_NORTH;
            }
        } else {
            if (x3 > x2) {
                return WEST_SOUTH;
            } else {
                return EAST_SOUTH;
            }
        }
    } else {
        if (x1 < x2) {
            if (x3 > x2) {
                return EAST_NORTH;
            } else {
                return WEST_NORTH;
            }
        } else {
            if (x3 > x2) {
                return EAST_SOUTH;
            } else {
                return WEST_SOUTH;
            }
        }
    }
}

void concave_point(polygon_set_type& polygon_set_result) {
    std::vector<polygon_type> polygons;
    polygon_set_result.get(polygons);
    std::cout << "minimum_rectangle" << std::endl;
    // polygon_90_with_holes_data<int>::iterator_type it;
    std::vector<std::vector<std::pair<point_type, int>>> concave_point_set(
        polygons.size());
    for (int i = 0; i < polygons.size(); i++) {
        // std::cout << "winding " <<
        // view_as<polygon_90_concept>(polygons[i]).winding() << std::endl;
        concave_point_set[i] = std::vector<std::pair<point_type, int>>();
        // do until the point before end
        auto it_before_2 = polygons[i].end();
        auto it_before_1 = polygons[i].begin();
        auto it = polygons[i].begin();
        it++;
        for (; it != polygons[i].end(); ++it) {
            std::cout << "in loop\n";
            std::cout << (*it_before_1).x() << " " << (*it_before_1).y() << " "
                      << available_directions(
                             (*it_before_2).x(), (*it_before_1).x(), (*it).x(),
                             (*it_before_2).y(), (*it_before_1).y(), (*it).y())
                      << "\n";
            if (is_concave((*it_before_2).x(), (*it_before_1).x(), (*it).x(),
                           (*it_before_2).y(), (*it_before_1).y(), (*it).y())) {
                std::cout << "concave!" << std::endl;
                concave_point_set[i].push_back(std::pair<point_type, int>(
                    *it_before_1,
                    available_directions((*it_before_2).x(), (*it_before_1).x(),
                                         (*it).x(), (*it_before_2).y(),
                                         (*it_before_1).y(), (*it).y())));
            }
            it_before_2 = it_before_1;
            it_before_1 = it;
        }
        assert(it == polygons[i].end());
        it = polygons[i].begin();

        // std::cout << "it: \n";
        // std::cout << (*it).x() << " " << (*it).y() << "\n";
        std::cout << (*it_before_1).x() << " " << (*it_before_1).y() << " "
                  << available_directions(
                         (*it_before_2).x(), (*it_before_1).x(), (*it).x(),
                         (*it_before_2).y(), (*it_before_1).y(), (*it).y())
                  << "\n";
        if (is_concave((*it_before_2).x(), (*it_before_1).x(), (*it).x(),
                       (*it_before_2).y(), (*it_before_1).y(), (*it).y())) {
            std::cout << "concave!" << std::endl;
            std::cout << (*it_before_1).x() << " " << (*it_before_1).y()
                      << "\n";
            concave_point_set[i].push_back(std::pair<point_type, int>(
                *it_before_1,
                available_directions((*it_before_2).x(), (*it_before_1).x(),
                                     (*it).x(), (*it_before_2).y(),
                                     (*it_before_1).y(), (*it).y())));
        }
        /*
        it_before_2 = it_before_1;
        it_before_1 = it;
        it = polygons[i].begin();
        // std::cout << "it: \n";
        // std::cout << (*it).x() << " " << (*it).y() << "\n";
        std::cout << (*it_before_1).x() << " " << (*it_before_1).y() << "\n";
        if (is_concave((*it_before_2).x(), (*it_before_1).x(), (*it).x(),
                       (*it_before_2).y(), (*it_before_1).y(), (*it).y())) {
            std::cout << "concave!" << std::endl;
            std::cout << (*it_before_1).x() << " " << (*it_before_1).y()
                      << "\n";
            concave_point_set[i].push_back(std::pair<point_type, int>(
                *it_before_1,
                available_directions((*it_before_2).x(), (*it_before_1).x(),
                                     (*it).x(), (*it_before_2).y(),
                                     (*it_before_1).y(), (*it).y())));
        }
        it_before_2 = it_before_1;
        it_before_1 = it;
        std::advance(it, 1);
        // std::cout << "it: \n";
        // std::cout << (*it).x() << " " << (*it).y() << "\n";
        std::cout << (*it_before_1).x() << " " << (*it_before_1).y() << "\n";
        if (is_concave((*it_before_2).x(), (*it_before_1).x(), (*it).x(),
                       (*it_before_2).y(), (*it_before_1).y(), (*it).y())) {
            std::cout << "concave!" << std::endl;
            std::cout << (*it_before_1).x() << " " << (*it_before_1).y()
                      << "\n";
            concave_point_set[i].push_back(std::pair<point_type, int>(
                *it_before_1,
                available_directions((*it_before_2).x(), (*it_before_1).x(),
                                     (*it).x(), (*it_before_2).y(),
                                     (*it_before_1).y(), (*it).y())));
        }
        */
        // add hole point to concave set
        std::cout << "hole point \n";
        for (auto it = polygons[i].begin_holes(); it != polygons[i].end_holes();
             ++it) {
            auto it_h = it->begin();
            for (auto it_h = it->begin(); it_h != it->end(); ++it_h) {
                std::cout << (*it_h).x() << " " << (*it_h).y() << "\n";
            }
        }

        for (auto it = polygons[i].begin_holes(); it != polygons[i].end_holes();
             ++it) {
            auto it_before_2 = it->end();

            std::cout << (*it_before_2).x() << " " << (*it_before_2).y()
                      << "\n";
            auto it_before_1 = it->begin();
            auto it_h = it->begin();
            ++it_h;
            for (; it_h != it->end(); ++it_h) {
                std::cout << "in loop " << std::endl;
                std::cout << (*it_before_2).x() << " " << (*it_before_2).y()
                          << "\n";
                std::cout << (*it_before_1).x() << " " << (*it_before_1).y()
                          << "\n";
                std::cout << (*it_h).x() << " " << (*it_h).y() << "\n";
                std::cout << available_directions(
                                 (*it_before_2).x(), (*it_before_1).x(),
                                 (*it_h).x(), (*it_before_2).y(),
                                 (*it_before_1).y(), (*it_h).y())
                          << "\n";

                concave_point_set[i].push_back(std::pair<point_type, int>(
                    *it_before_1,
                    available_directions((*it_before_2).x(), (*it_before_1).x(),
                                         (*it_h).x(), (*it_before_2).y(),
                                         (*it_before_1).y(), (*it_h).y())));
                it_before_2 = it_before_1;
                it_before_1 = it_h;
            }
            assert(it_h == it->end());
            it_h = it->begin();
            std::cout << (*it_before_1).x() << " " << (*it_before_1).y() << " "
                      << available_directions((*it_before_2).x(),
                                              (*it_before_1).x(), (*it_h).x(),
                                              (*it_before_2).y(),
                                              (*it_before_1).y(), (*it_h).y())
                      << "\n";
            concave_point_set[i].push_back(std::pair<point_type, int>(
                *it_before_1,
                available_directions((*it_before_2).x(), (*it_before_1).x(),
                                     (*it_h).x(), (*it_before_2).y(),
                                     (*it_before_1).y(), (*it_h).y())));
            /*
            it_before_2 = it_before_1;
            it_before_1 = it_h;
            it_h = it->begin();
            std::cout << (*it_before_1).x() << " " << (*it_before_1).y()
                      << "\n";
            concave_point_set[i].push_back(std::pair<point_type, int>(
                *it_before_1,
                available_directions((*it_before_2).x(), (*it_before_1).x(),
                                     (*it_h).x(), (*it_before_2).y(),
                                     (*it_before_1).y(), (*it_h).y())));
            it_before_2 = it_before_1;
            it_before_1 = it_h;
            ++it_h;
            std::cout << (*it_before_1).x() << " " << (*it_before_1).y()
                      << "\n";
            concave_point_set[i].push_back(std::pair<point_type, int>(
                *it_before_1,
                available_directions((*it_before_2).x(), (*it_before_1).x(),
                                     (*it_h).x(), (*it_before_2).y(),
                                     (*it_before_1).y(), (*it_h).y())));
            */
        }
        std::cout << "concave_point_set " << i << std::endl;
        for (int j = 0; j < concave_point_set[i].size(); j++) {
            std::cout << "mode" << concave_point_set[i][j].second << std::endl;
            std::cout << (concave_point_set[i][j]).first.x() << " "
                      << (concave_point_set[i][j]).first.y() << std::endl;
        }
    }
}
