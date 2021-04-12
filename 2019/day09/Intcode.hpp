#include <iostream>
#include <array>
#include <thread>
#include <mutex>
#include <cstdint>

namespace Intcode
{
  using opcode = std::array<std::int64_t, 4>;
  opcode separate(std::int64_t value)
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

  struct waiting_for_input : public std::exception
  {
    const char* what() const noexcept override { return "Waiting on input"; }
  };

  struct program_exit : public std::exception
  {
    const char* what() const noexcept override { return "Program exited"; }
  };

  struct Queue
  {
    virtual void push(std::int64_t value) = 0;
    virtual std::int64_t pop() = 0;
    virtual bool empty() = 0;
  };

  struct ConsoleQueue : public Queue
  {
    ConsoleQueue()
      : input(std::cin), output(std::cout)
    {}

    ConsoleQueue(std::istream& input, std::ostream& output)
      : input(input), output(output)
    {}

    void push(std::int64_t value) override
    {
      std::cout << value << std::endl;
    }

    std::int64_t pop() override
    {
      std::int64_t value;
      std::cout << "Enter value: ";
      std::cin >> value;
      return value;
    }

    bool empty() override { return false; }

    std::istream& input;
    std::ostream& output;
  };

  struct ExceptionReentrantQueue : public Queue
  {
    ExceptionReentrantQueue() = default;
    ExceptionReentrantQueue(std::vector<std::int64_t> queue)
      : queue(queue)
    {}
    
    void push(std::int64_t value) override
    {
      queue.push_back(value);
    }

    std::int64_t pop() override
    {
      if(queue.empty())
	{
	  throw waiting_for_input {};
	}

      std::int64_t value = queue.front();
      queue.erase(queue.begin());
      return value;
    }

    bool empty() override
    {
      return queue.empty();
    }

    std::vector<std::int64_t> queue;
  };

  struct ThreadedBlockingQueue : public Queue
  {
    ThreadedBlockingQueue() = default;
    ThreadedBlockingQueue(std::vector<std::int64_t> queue)
      : queue(queue)
    {}

    void push(std::int64_t value) override
    {
      std::lock_guard lock(mtx);
      queue.push_back(value);
    }

    std::int64_t pop() override
    {
      using namespace std::chrono_literals;
      while(queue.empty())
	{
	  std::this_thread::sleep_for(1ns);
	}

      std::lock_guard lock(mtx);
      std::int64_t value = queue.front();
      queue.erase(queue.begin());
      return value;
    }

    bool empty() override
    {
      std::lock_guard lock(mtx);
      return queue.empty();
    }

    std::mutex mtx;
    std::vector<std::int64_t> queue;
  };


  struct Registers
  {
    std::int64_t _ip = 0;
    std::int64_t _relative_base = 0;

    std::int64_t ip() { return _ip; }
    void ip(std::int64_t new_ip) { _ip = new_ip; }
    void advanceIP(std::int64_t offset) { _ip += offset; }

    std::int64_t relativeBase() { return _relative_base; }
    void addRelative(std::int64_t offset) { _relative_base += offset; }
  };

    
  struct Context
  {
    Context(std::vector<std::int64_t>& memory, Registers& registers, Queue& input, Queue& output)
      : memory(memory), registers(registers), input(input), output(output), op(separate(immediate<0>()))
    {}

    std::int64_t code() const { return op[0]; }
    std::int64_t ip() const { return registers.ip(); }

    template<std::int64_t Index>
    std::int64_t& immediate() { return memory[registers.ip() + Index]; }

    template<std::int64_t Index>
    std::int64_t& reference() { return memory[immediate<Index>()]; }

    template<std::int64_t Index>
    std::int64_t& relative() { return memory[registers.relativeBase() + immediate<Index>()]; }
  
    template<std::int64_t Index>
    std::int64_t& param()
    {
      switch(op[Index])
	{
	case 0:
	  return reference<Index>();
	case 1:
	  return immediate<Index>();
	case 2:
	  return relative<Index>();
	default:
	  throw std::runtime_error("Unknown parameter base");
	}
    }
    
    std::vector<std::int64_t>& memory;
    Registers& registers;
    Queue& input;
    Queue& output;
    opcode op;
  };

  namespace Opcode
  {
    namespace Impl
    {
      void invalid(Context c)
      {
	throw std::runtime_error("Invalid opcode");
      }

      void add(Context c)
      {
	c.param<3>() = c.param<1>() + c.param<2>();
	c.registers.advanceIP(4);
      }

      void mult(Context c)
      {
	c.param<3>() = c.param<1>() * c.param<2>();
	c.registers.advanceIP(4);
      }
      
      void input(Context c)
      {
	c.param<1>() = c.input.pop();
	c.registers.advanceIP(2);
      }
      
      void output(Context c)
      {
	c.output.push(c.param<1>());
	c.registers.advanceIP(2);
      }
      
      void jumpTrue(Context c)
      {
	if (c.param<1>() != 0)
	  {
	    c.registers.ip(c.param<2>());
	  }
	else
	  {
	    c.registers.advanceIP(3);
	  }
      }
      
      void jumpFalse(Context c)
      {
	if (c.param<1>() == 0)
	  {
	    c.registers.ip(c.param<2>());
	  }
	else
	  {
	    c.registers.advanceIP(3);
	  }
      }
      
      void lessThan(Context c)
      {
	c.param<3>() = c.param<1>() < c.param<2>() ? 1 : 0;
	c.registers.advanceIP(4);
      }
      
      void equal(Context c)
      {
	c.param<3>() = c.param<1>() == c.param<2>() ? 1 : 0;
	c.registers.advanceIP(4);
      }

      void adjustRelative(Context c)
      {
	c.registers.addRelative(c.param<1>());
	c.registers.advanceIP(2);
      }
    }


    using Func = void (*)(Context);
    constexpr std::array<Func, 10> Map { Impl::invalid,     // 0
					Impl::add,         // 1 - Add
					Impl::mult,        // 2 - Multiply
					Impl::input,       // 3 - Console input value
					Impl::output,      // 4 - Console output value
					Impl::jumpTrue,    // 5 - Jump if true
					Impl::jumpFalse,   // 6 - Jump if false
					Impl::lessThan,    // 7 - Less Than
					Impl::equal,       // 8 - Equal to
					Impl::adjustRelative, // 9 - Adjust relative base
    };

    void testCode(std::int64_t code)
    {
      if(code <= 0 || code >= Map.size())
	{
	  throw std::runtime_error("Invalid opcode");
	}
    }
  }

  struct Machine
  {
    Machine(std::vector<std::int64_t> _memory, Queue& input, Queue& output)
      : memory(_memory.size() * 50), input(input), output(output)
    {
      std::copy(_memory.begin(), _memory.end (), memory.begin());
    }

    void run()
    {
      while(memory.at(registers.ip()) != 99)
	{
	  Context c { memory, registers, input, output };
	  //	  std::cout << registers.ip() << ":" << c.immediate<0>() << " " << c.immediate<1>() << " " << c.immediate<2>() << " " << c.immediate<2>() << std::endl;
	  Opcode::testCode(c.code());
	  Opcode::Map[c.code()](c);
	}
    }

    std::vector<std::int64_t> memory;
    Queue& input;
    Queue& output;
    Registers registers;
  };
}
