#include <iostream>
#include <array>
#include <vector>

namespace Intcode
{
  using opcode = std::array<int, 4>;
  opcode separate(int value)
  {
    return { (value % 100),
	     ((value / 100) % 10),
	     ((value / 1000) % 10),
	     ((value / 10000) % 10) };
  }

  std::ostream& operator<<(std::ostream& out, const opcode& op)
  {
    out << op[3] << op[2] << op[1] << op[0];
    return out;
  }

  struct Registers
  {
    int _ip = 0;

    int ip() { return _ip; }
    void ip(int new_ip) { _ip = new_ip; }
    void advanceIP(int offset) { _ip += offset; }
  };

  
  template<typename InputIterator, typename OutputIterator>
  struct Context
  {
    Context(std::vector<int>& memory, Registers& registers, InputIterator input_iter, OutputIterator output_iter)
      : memory(memory),
	registers(registers),
	input_iter(input_iter),
	output_iter(output_iter),
	op(separate(immediate(0)))
    {}

    int code() const { return op[0]; }
    int ip() const { return registers.ip(); }


    int& immediate(int index) { return memory.at(registers.ip() + index); }
    int& reference(int index) { return memory.at(immediate(index)); }
    int& relative(int index) { return memory.at(registers.ip() + immediate(index)); }
    int& param(int index)
    {
      return op[index] != 0 ? immediate(index) : reference(index);    
    }
    
    std::vector<int>& memory;
    Registers& registers;
    InputIterator input_iter;
    OutputIterator output_iter;
    opcode op;
  };

  namespace Opcode
  {
    template<typename InputIterator, typename OutputIterator>
    class Impl
    {
    public:
      static void invalid(Context<InputIterator,OutputIterator> c)
      {
	throw std::runtime_error("Invalid opcode");
      }

      static void add(Context<InputIterator,OutputIterator> c)
      {
	c.param(3) = c.param(1) + c.param(2);
	c.registers.advanceIP(4);
      }

      static void mult(Context<InputIterator,OutputIterator> c)
      {
	c.param(3) = c.param(1) * c.param(2);
	c.registers.advanceIP(4);
      }
      
      static void input(Context<InputIterator,OutputIterator> c)
      {
	c.param(1) = *(c.input_iter);
	++(c.input_iter);
	c.registers.advanceIP(2);
      }
      
      static void output(Context<InputIterator,OutputIterator> c)
      {
	//	std::cout << c.param(1);
	//	int value = c.param(1);
	//	*(c.output_iter) = value;
	//	++c.output_iter;
	c.registers.advanceIP(2);
      }
      
      static void jumpTrue(Context<InputIterator,OutputIterator> c)
      {
	if (c.param(1) != 0)
	  {
	    c.registers.ip(c.param(2));
	  }
	else
	  {
	    c.registers.advanceIP(3);
	  }
      }
      
      static void jumpFalse(Context<InputIterator,OutputIterator> c)
      {
	if (c.param(1) == 0)
	  {
	    c.registers.ip(c.param(1));
	  }
	else
	  {
	    c.registers.advanceIP(3);
	  }
      }
      
      static void lessThan(Context<InputIterator,OutputIterator> c)
      {
	c.param(3) = c.param(1) < c.param(2) ? 1 : 0;
	c.registers.advanceIP(4);
      }
      
      static void equal(Context<InputIterator,OutputIterator> c)
      {
	c.param(3) = c.param(1) < c.param(2) ? 1 : 0;
	c.registers.advanceIP(4);
      }
    };


  }

  template<typename InputIterator, typename OutputIterator>
  struct Machine
  {
    Machine(std::vector<int> memory, InputIterator input_iter, OutputIterator output_iterator)
      : memory(memory), input_iter(input_iter), output_iter(output_iter)
    {}

    void run()
    {
      while(memory.at(registers.ip()) != 99)
	{
	  Context<InputIterator, OutputIterator> c { memory, registers, input_iter, output_iter };
	  testCode(c.code());
	  Map[c.code()](c);
	}
    }

    void testCode(int code)
    {
      if(code <= 0 || code >= Map.size())
	{
	  throw std::runtime_error("Invalid opcode");
	}
    }

    using Func = void (*)(Context<InputIterator, OutputIterator>);
    static constexpr std::array<Func, 9> Map { Opcode::Impl<InputIterator,OutputIterator>::invalid,     // 0
					       Opcode::Impl<InputIterator,OutputIterator>::add,         // 1 - Add
					       Opcode::Impl<InputIterator,OutputIterator>::mult,        // 2 - Multiply
					       Opcode::Impl<InputIterator,OutputIterator>::input,       // 3 - Console input value
					       Opcode::Impl<InputIterator,OutputIterator>::output,      // 4 - Console output value
					       Opcode::Impl<InputIterator,OutputIterator>::jumpTrue,    // 5 - Jump if true
					       Opcode::Impl<InputIterator,OutputIterator>::jumpFalse,   // 6 - Jump if false
					       Opcode::Impl<InputIterator,OutputIterator>::lessThan,    // 7 - Less Than
					       Opcode::Impl<InputIterator,OutputIterator>::equal,       // 8 - Equal to
    };

    Registers registers;
    std::vector<int> memory;
    InputIterator input_iter;
    OutputIterator output_iter;
  };
}
