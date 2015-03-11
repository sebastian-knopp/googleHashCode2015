#include "GreedySolver.h"


GreedySolver::GreedySolver(const base::Vector2d<State>& a_vector)
: m_targetCanvas(a_vector)
{
    m_currentCanvas = base::Vector2d<State>(m_targetCanvas.width(), m_targetCanvas.height(), State::White);
}


Commands GreedySolver::solve()
{
    Commands result;

    bool done = [&] () {
        for (size_t x = 0; x != m_targetCanvas.width(); ++x)
        {
            for (size_t y = 0; y !=m_targetCanvas.height(); ++y)
            {
                if (m_targetCanvas(x, y) == State::Black)
                    return false;
            }
        }
        return true;
    }();

    while (!done)
    {
        //size_t squareSize = std::min(m_targetCanvas.width(), m_targetCanvas.height());
        size_t squareSize = 3;
        if (squareSize % 2 == 0)
            --squareSize;

        static const int minusInfinity = -99;
        size_t S = (squareSize -1) / 2;

        int maximumScore = minusInfinity;

        Command newCommand(Command::Type::PaintSQ, 0, 0, 0);

        base::Vector2d<int> scoreTable(m_targetCanvas.width(), m_targetCanvas.height(), minusInfinity);
        while (2*S + 1 > 0)
        {
            for (size_t x = S; x < m_targetCanvas.width() - S; ++x)
            {
                for (size_t y = S; y < m_targetCanvas.height() - S; ++y)
                {
                    scoreTable(x, y)  = computeCellScore(x, y, S);
                    if (scoreTable(x, y) > maximumScore)
                    {
                        maximumScore = scoreTable(x, y);
                        newCommand.m_x = x;
                        newCommand.m_y = y;
                        newCommand.m_s = S;
                    }
                }
            }

//            std::cout << "scoretable:" << std::endl;
//            std::cout << scoreTable << std::endl;

            if (S == 0)
                break;

            --S;
        }

        if (maximumScore <= 0)
            done = true;
        else
        {
            result.push_back(newCommand);
            newCommand.execute(m_currentCanvas);

//            std::cout << "---" << std::endl;
            std::cout << newCommand << std::endl;
//            std::cout << m_currentCanvas << std::endl;
        }
    }

    for (size_t x = 0; x != m_targetCanvas.width(); ++x)
    {
        for (size_t y = 0; y !=m_targetCanvas.height(); ++y)
        {
            if (m_targetCanvas(x, y) == State::White &&
                m_currentCanvas(x, y) == State::Black)
                result.emplace_back(Command::Type::EraseCell, x, y, 0);
        }
    }

    return result;
}


int GreedySolver::computeCellScore(size_t a_x, size_t a_y, size_t a_S)
{
    int score = 0;
    for (size_t x = a_x - a_S; x <= a_x + a_S; ++x)
    {
        for (size_t y = a_y - a_S; y <= a_y + a_S; ++y)
        {
            if (m_currentCanvas(x, y) == State::White &&
                m_targetCanvas(x, y) == State::Black)
                ++score;

            if (m_currentCanvas(x, y) == State::White && // TODO: perhaps do count also if it was white
                m_targetCanvas(x, y) == State::White)
                --score;
        }
    }
    return score;
}


Commands solveGreedily(const base::Vector2d<State>& a_vector)
{
    GreedySolver solver(a_vector);
    return solver.solve();
}


