#include <iostream>
#include <assert.h>

/*
Board template-class

Container is template class which takes 2 typenames
value_type is type of data stored in Container
*/

template <template <typename, typename> class Container, typename value_type>
class Board{
public:
    typedef Container<value_type, std::allocator<value_type>> row_type; // type holding one row of the board ( second dimension )
    typedef Container<row_type, std::allocator<row_type>> board_type;   // type holding all rows of the board ( first dimension )

    Board(int first_dimension_size, int second_dimension_size, value_type default_val)    
    {
        assert(first_dimension_size > 0 && second_dimension_size > 0);
        container = board_type(first_dimension_size, 
                  row_type(second_dimension_size, default_val));
    }

    template <template <typename, typename> class C, typename V>
    friend std::ostream& operator<<(std::ostream& os, const Board<C, V>& b);
private:
    board_type container;
};

template <template <typename, typename> class Container, typename value_type>
std::ostream& operator<<(std::ostream& os, const Board<Container, value_type>& b)
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