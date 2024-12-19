#include <iostream>
#include <assert.h>

/*
Board template-class

Container is template class which takes 2 typenames
value_type is type of data stored in Container
*/

template <template <typename, typename> class Container, typename value_type, template <typename> class Alloc>
class Board{
public:
    typedef Container<value_type, Alloc<value_type>> row_type; // type holding one row of the board ( second dimension )
    typedef Container<row_type, Alloc<row_type>> board_type;   // type holding all rows of the board ( first dimension )

    Board(int first_dimension_size, int second_dimension_size, value_type default_val)    
    {
        assert(first_dimension_size > 0 && second_dimension_size > 0);
        container = board_type(first_dimension_size, 
                  row_type(second_dimension_size, default_val));
    }

    template <template <typename, typename> class C, typename V, template <typename> class A>
    friend std::ostream& operator<<(std::ostream& os, const Board<C, V, A>& b);
private:
    board_type container;
};

template <template <typename, typename> class C, typename V, template <typename> class A>
std::ostream& operator<<(std::ostream& os, const Board<C, V, A>& b)
{
    for (const auto& first_dimension_container : b.container)
    {
        for (const auto& value : first_dimension_container)
        {
            os << value << " ";
        }
        os << "\n";
    }
    return os;
}
