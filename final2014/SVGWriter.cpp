#include "SVGWriter.h"
#include <iostream>


SVGWriter::SVGWriter(const std::string& a_filename)
: m_ofs(a_filename)
{
}


SVGWriter::~SVGWriter()
{
    const double dblWidth = m_max.m_x - m_min.m_x;
    double dblHeight = m_max.m_y - m_min.m_y;

    double scaleFactor = static_cast<double>(TARGET_SIZE) / std::max(dblWidth, dblHeight);

    auto getScaledValue = [scaleFactor] (const double a_value)
    {
        return static_cast<int>(scaleFactor * a_value);
    };

    const int width = getScaledValue(dblWidth) + 2 * BORDER_WIDTH;
    const int height = getScaledValue(dblHeight) + 2 * BORDER_WIDTH;


    m_ofs << "<!DOCTYPE html>\n"
          << "<html>\n"
          << "<body>\n"
          << "<svg height=\"" << height << "\" width=\"" << width << "\">\n";

    for (const DrawCommand& command : m_commands)
    {
        switch (command.m_type)
        {
            case CommandType::Circle:
                m_ofs << "<circle "
                      << "cx=\"" << (getScaledValue(command.m_fromCoord.m_x - m_min.m_x) + BORDER_WIDTH) << "\" "
                      << "cy=\"" << (getScaledValue(command.m_fromCoord.m_y - m_min.m_y) + BORDER_WIDTH) << "\" "
                      << "r=\"2\" stroke=\"black\" stroke-width=\"0\""
                      << "fill=\"" << getColorString(command.m_color) << "\" />\n";
                break;
            case CommandType::Line:
                m_ofs << "<line "
                      << "x1=\"" << getScaledValue(command.m_fromCoord.m_x - m_min.m_x) + BORDER_WIDTH << "\" "
                      << "y1=\"" << getScaledValue(command.m_fromCoord.m_y - m_min.m_y) + BORDER_WIDTH << "\" "
                      << "x2=\"" << getScaledValue(command.m_toCoord.m_x - m_min.m_x) + BORDER_WIDTH << "\" "
                      << "y2=\"" << getScaledValue(command.m_toCoord.m_y - m_min.m_y) + BORDER_WIDTH << "\" "
                      << " style=\"stroke:" << getColorString(command.m_color) << ";stroke-width:1\" />\n";
                break;
        }
    }

    m_ofs << "</svg>\n"
          << "</body>\n"
          << "</html>\n";
}


void SVGWriter::drawCircle(double a_x, double a_y, int a_color)
{
    const Coordinate c {  a_x, a_y };
    updateMinMax(c);

    m_commands.push_back(DrawCommand { CommandType::Circle, c, c, a_color });
}


void SVGWriter::drawLine(double a_fromX, double a_fromY, double a_toX, double a_toY, int a_color)
{
    const Coordinate from {  a_fromX, a_fromY };
    updateMinMax(from);

    const Coordinate to {  a_toX, a_toY };
    updateMinMax(to);

    m_commands.push_back(DrawCommand { CommandType::Line, from, to, a_color });
}


std::string SVGWriter::getColorString(int a_color)
{
    switch (a_color)
    {
        case 0:
            return "black";
        case 1:
            return "darkgreen";
        case 2:
            return "lightskyblue";
        case 3:
            return "hotpink";
        case 4:
            return "mediumpurple";
        case 5:
            return "goldenrod";
        case 6:
            return "slategrey";
        case 7:
            return "gold";
        case 8:
            return "red";
        default:
            return "darkcyan";
    }
}


void SVGWriter::updateMinMax(const SVGWriter::Coordinate& a_coord)
{
    m_min.m_x = std::min(a_coord.m_x, m_min.m_x);
    m_min.m_y = std::min(a_coord.m_y, m_min.m_y);

    m_max.m_x = std::max(a_coord.m_x, m_max.m_x);
    m_max.m_y = std::max(a_coord.m_y, m_max.m_y);
}





