#ifndef SLICE_H
#define SLICE_H
#include <stdint.h>

struct Slice
{
    size_t m_row1;
    size_t m_column1;
    size_t m_row2;
    size_t m_column2;

    size_t getNmbCells() const;

};


#endif // SLICE_H
