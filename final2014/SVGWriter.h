#pragma once
#include <fstream>
#include <vector>
#include <limits>


class SVGWriter {

    static const int TARGET_SIZE = 800;
    static const int BORDER_WIDTH = 10;


public:

    explicit SVGWriter(const std::string& a_filename);

    ~SVGWriter();

    void drawCircle(double a_x, double a_y, int a_color, int a_radius);

    void drawLine(double a_fromX, double a_fromY, double a_toX, double a_toY, int a_color);

    static std::string getColorString(int a_color);

private:

    struct Coordinate
    {
        double m_x ;
        double m_y;
    };

    enum class CommandType
    {
        Circle,
        Line
    };

    struct DrawCommand
    {
        CommandType m_type;
        Coordinate m_fromCoord;
        Coordinate m_toCoord;
        int m_color;
        int m_radius;
    };

    void updateMinMax(const Coordinate& a_coord);


private:

    std::vector<DrawCommand> m_commands;

    std::ofstream m_ofs;

    Coordinate m_min = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max() } ;
    Coordinate m_max = { 0, 0 } ;

};

