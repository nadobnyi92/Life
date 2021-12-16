#include <iostream>

#include <chrono>
#include <thread>

constexpr size_t filed_size = 31;
const char dead[] = "\033[1;31mx\033[0m";;
const char life[] = "\033[1;32mo\033[0m";;

template <typename T, int N>
int size( T(&a)[N] ) { return N; }

class LifeMachine
{
public:
    typedef std::array< std::array< bool, filed_size >, filed_size > StateMatrix;

    LifeMachine(StateMatrix startState = {});

    void step();
    void print();
    bool isFinish();

private:
    bool isLife( size_t row, size_t col );

private:
    StateMatrix m_state; 
};

LifeMachine::LifeMachine(StateMatrix startState )
    : m_state( startState )
{
}

void LifeMachine::step()
{
    StateMatrix newState;

    for( size_t i = 0; i < filed_size; ++i )
    {
        for( size_t j = 0; j < filed_size; ++j )
        {
            newState[i][j] = isLife( i, j );
        }
    }

    m_state = newState;
}

void LifeMachine::print()
{
    std::cout << "\x1B[2J\x1B[H";
    for( size_t i = 0; i < filed_size; ++i )
    {
        for( size_t j = 0; j < filed_size; ++j )
        {
            std::cout << ( m_state[i][j] ? life : dead );
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}

bool LifeMachine::isLife( size_t row, size_t col )
{
    size_t life_alongside = 0;
    constexpr int StepMatrix[] = { -1, 0, 1 };
    for( size_t i = 0; i < size( StepMatrix ); ++i )
    {
        for( size_t j = 0; j < size( StepMatrix ); ++j )
        {
            if( StepMatrix[i] == 0 && StepMatrix[j] == 0 ) continue;
            size_t neghtborRow = ( row + filed_size + StepMatrix[i] ) % filed_size;
            size_t neghtborCol = ( col + filed_size + StepMatrix[j] ) % filed_size;
            if( m_state[neghtborRow][neghtborCol] ) ++life_alongside;
        }
    }
    return life_alongside == 2 || life_alongside == 3;
}

bool LifeMachine::isFinish()
{
    return false;
}

LifeMachine::StateMatrix signal()
{
    LifeMachine::StateMatrix state {};
    for( size_t i = 0; i < filed_size; ++i)
    {
        for( size_t j = 0; j < filed_size; ++j )
        {
            state[i][j] = (i % 3 == 2) && ( i % 2 == 0 ? (j % 6 < 3) : (j % 6 >= 3) );
        }
    }
    return state;
}

int main()
{
    LifeMachine machine { signal() };
    while( !machine.isFinish() )
    {
        char ch;
        machine.print();
        machine.step();
        std::cin >>  ch;
        // std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }
    return 0;
}
