#include "Slice.h"



size_t Slice::getNmbCells() const
{
    return (m_row2 - m_row1 + 1) * (m_column2 - m_column1 + 1);
}

