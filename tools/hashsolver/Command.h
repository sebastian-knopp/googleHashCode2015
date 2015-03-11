#pragma once
#include "Canvas.h"

struct Command
{
    enum class  Type {
        PaintSQ,
        EraseCell
    };

    Command(Type a_type,
            size_t a_x,
            size_t a_y,
            size_t a_s);

    void execute(base::Vector2d<State>& a_vector) const;


public:
    Type m_type = Type::PaintSQ;
    size_t m_x = 0;
    size_t m_y = 0;
    size_t m_s = 0;
};

typedef std::vector<Command> Commands;


std::ostream& operator<<(std::ostream& os, const Command& a_command);


void verify(const base::Vector2d<State>& a_vector, Commands& a_commands);

