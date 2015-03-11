#pragma once
#include "Canvas.h"
#include "Command.h"


class GreedySolver {

public:

    GreedySolver(const base::Vector2d<State>& a_vector);

    Commands solve();

private:

    int computeCellScore(size_t x, size_t y, size_t S);


private:

    const base::Vector2d<State>& m_targetCanvas;

    base::Vector2d<State> m_currentCanvas;

};


Commands solveGreedily(const base::Vector2d<State>& a_vector);

