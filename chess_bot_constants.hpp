#include <array>

namespace cbot_constants
{
    const int MAX_MULTIPLIER = 5;

    const std::array<std::array<double, 8>, 8> multiplier_table_pawn
    {{
        { 0.0, 0.1, 0.1, 0.2, 0.2, 0.1, 0.1, 0.0 },
        { 0.1, 0.2, 0.3, 0.4, 0.4, 0.3, 0.2, 0.1 },
        { 0.1, 0.3, 0.4, 0.5, 0.5, 0.4, 0.3, 0.1 },
        { 0.2, 0.4, 0.5, 0.6, 0.6, 0.5, 0.4, 0.2 },
        { 0.2, 0.4, 0.6, 0.7, 0.7, 0.6, 0.4, 0.2 },
        { 0.2, 0.3, 0.4, 0.6, 0.6, 0.4, 0.3, 0.2 },
        { 0.1, 0.2, 0.3, 0.4, 0.4, 0.3, 0.2, 0.1 },
        { 0.0, 0.1, 0.1, 0.2, 0.2, 0.1, 0.1, 0.0 }
    }};

    const std::array<std::array<double, 8>, 8> multiplier_table_knight
    {{
        { -0.5, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.5 },
        { -0.4, 0.0, 0.2, 0.2, 0.2, 0.2, 0.0, -0.4 },
        { -0.4, 0.2, 0.4, 0.5, 0.5, 0.4, 0.2, -0.4 },
        { -0.4, 0.2, 0.5, 0.6, 0.6, 0.5, 0.2, -0.4 },
        { -0.4, 0.2, 0.5, 0.6, 0.6, 0.5, 0.2, -0.4 },
        { -0.4, 0.2, 0.4, 0.5, 0.5, 0.4, 0.2, -0.4 },
        { -0.4, 0.0, 0.2, 0.2, 0.2, 0.2, 0.0, -0.4 },
        { -0.5, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.5 }
    }};

    const std::array<std::array<double, 8>, 8> multiplier_table_bishop
    {{
        { -0.5, -0.3, -0.2, -0.1, -0.1, -0.2, -0.3, -0.5 },
        { -0.3, 0.0, 0.1, 0.3, 0.3, 0.1, 0.0, -0.3 },
        { -0.2, 0.1, 0.2, 0.4, 0.4, 0.2, 0.1, -0.2 },
        { -0.1, 0.3, 0.4, 0.6, 0.6, 0.4, 0.3, -0.1 },
        { -0.1, 0.3, 0.4, 0.6, 0.6, 0.4, 0.3, -0.1 },
        { -0.2, 0.1, 0.2, 0.4, 0.4, 0.2, 0.1, -0.2 },
        { -0.3, 0.0, 0.1, 0.3, 0.3, 0.1, 0.0, -0.3 },
        { -0.5, -0.3, -0.2, -0.1, -0.1, -0.2, -0.3, -0.5 }
    }};

    const std::array<std::array<double, 8>, 8> multiplier_table_rook
    {{
        { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
        { 0.2, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.2 },
        { 0.2, 0.4, 0.6, 0.6, 0.6, 0.6, 0.4, 0.2 },
        { 0.2, 0.4, 0.6, 0.8, 0.8, 0.6, 0.4, 0.2 },
        { 0.2, 0.4, 0.6, 0.8, 0.8, 0.6, 0.4, 0.2 },
        { 0.2, 0.4, 0.6, 0.6, 0.6, 0.6, 0.4, 0.2 },
        { 0.2, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.2 },
        { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }
    }};

    const std::array<std::array<double, 8>, 8> multiplier_table_queen
    {{
        { -0.5, -0.4, -0.4, -0.3, -0.3, -0.4, -0.4, -0.5 },
        { -0.4, -0.2, 0.0, 0.1, 0.1, 0.0, -0.2, -0.4 },
        { -0.4, 0.0, 0.2, 0.3, 0.3, 0.2, 0.0, -0.4 },
        { -0.3, 0.1, 0.3, 0.5, 0.5, 0.3, 0.1, -0.3 },
        { -0.3, 0.1, 0.3, 0.5, 0.5, 0.3, 0.1, -0.3 },
        { -0.4, 0.0, 0.2, 0.3, 0.3, 0.2, 0.0, -0.4 },
        { -0.4, -0.2, 0.0, 0.1, 0.1, 0.0, -0.2, -0.4 },
        { -0.5, -0.4, -0.4, -0.3, -0.3, -0.4, -0.4, -0.5 }
    }};

    const std::array<std::array<double, 8>, 8> multiplier_table_king
    {{
        { -0.6, -0.6, -0.6, -0.6, -0.6, -0.6, -0.6, -0.6 },
        { -0.6, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.6 },
        { -0.6, -0.3, -0.2, -0.2, -0.2, -0.2, -0.3, -0.6 },
        { -0.6, -0.3, -0.2, 0.0, 0.0, -0.2, -0.3, -0.6 },
        { -0.6, -0.3, -0.2, 0.0, 0.0, -0.2, -0.3, -0.6 },
        { -0.6, -0.4, -0.3, -0.3, -0.3, -0.3, -0.4, -0.6 },
        { -0.6, -0.6, -0.6, -0.6, -0.6, -0.6, -0.6, -0.6 },
        { -0.6, -0.6, -0.6, -0.6, -0.6, -0.6, -0.6, -0.6 }
    }};
}