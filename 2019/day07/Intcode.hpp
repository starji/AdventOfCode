#include <iostream>
#include <array>
#include <thread>
#include <mutex>

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
    virtual void push(int value) = 0;
    virtual int pop() = 0;
    virtual bool empty() = 0;
    virtual int front() = 0;
  };

  struct ExceptionReentrantQueue : public Queue
  {
    ExceptionReentrantQueue() = default;
    ExceptionReentrantQueue(std::vector<int> queue)
      : queue(queue)
    {}
    
    void push(int value) override
    {
      queue.push_back(value);
    }

    int pop() override
    {
      if(queue.empty())
	{
	  throw waiting_for_input {};
	}

      int value = queue.front();
      queue.erase(queue.begin());
      return value;
    }

    bool empty() override
    {
      return queue.empty();
    }

    int front() override
    {
      return queue.front();
    }

    std::vector<int> queue;
  };

  struct ThreadedBlockingQueue : public Queue
  {
    ThreadedBlockingQueue() = default;
    ThreadedBlockingQueue(std::vector<int> queue)
      : queue(queue)
    {}

    void push(int value) override
    {
      std::lock_guard lock(mtx);
      queue.push_back(value);
    }

    int pop() override
    {
      using namespace std::chrono_literals;
      while(queue.empty())
	{
	  std::this_thread::sleep_for(1ns);
	}

      std::lock_guard lock(mtx);
      int value = queue.front();
      queue.erase(queue.begin());
      return value;
    }

    bool empty() override
    {
      std::lock_guard lock(mtx);
      return queue.empty();
    }

    int front() override
    {
      return queue.front();
    }

    std::mutex mtx;
    std::vector<int> queue;
  };


  struct Registers
  {
    int _ip = 0;

    int ip() { return _ip; }
    void ip(int new_ip) { _ip = new_ip; }
    void advanceIP(int offset) { _ip += offset; }
  };

    
  struct Context
  {
    Context(std::vector<int>& memory, Registers& registers, Queue& input, Queue& output)
      : memory(memory), registers(registers), input(input), output(output), op(separate(immediate<0>()))
    {}

    int code() const { return op[0]; }
    int ip() const { return registers.ip(); }

    template<int Index>
    int& immediate() { return memory.at(registers.ip() + Index); }

    template<int Index>
    int& reference() { return memory.at(immediate<Index>()); }

    template<int Index>
    int& relative() { return memory.at(registers.ip() + immediate<Index>()); }
  
    template<int Index>
    int& param()
    {
      return op[Index] != 0 ? immediate<Index>() : reference<Index>();    
    }
    
    std::vector<int>& memory;
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
	
	// c.param<1>() = c.input.front();
	// c.input.erase(c.input.begin());

	// std::cout << "Enter: ";
	// std::cin >> c.param<1>();
	c.registers.advanceIP(2);
      }
      
      void output(Context c)
      {
	c.output.push(c.param<1>());

	// c.output.push_back(c.param<1>());

	// std::cout << c.param<1>() << std::endl;

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
	c.param<3>() = c.param<1>() < c.param<2>() ? 1 : 0;
	c.registers.advanceIP(4);
      }
    }


    using Func = void (*)(Context);
    constexpr std::array<Func, 9> Map { Impl::invalid,     // 0
					Impl::add,         // 1 - Add
					Impl::mult,        // 2 - Multiply
					Impl::input,       // 3 - Console input value
					Impl::output,      // 4 - Console output value
					Impl::jumpTrue,    // 5 - Jump if true
					Impl::jumpFalse,   // 6 - Jump if false
					Impl::lessThan,    // 7 - Less Than
					Impl::equal,       // 8 - Equal to
    };

    void testCode(int code)
    {
      if(code <= 0 || code >= Map.size())
	{
	  throw std::runtime_error("Invalid opcode");
	}
    }
  }

  struct Machine
  {
    Machine(std::vector<int> memory, Queue& input, Queue& output)
      : memory(memory), input(input), output(output)
    {}

    void run()
    {
      while(memory.at(registers.ip()) != 99)
	{
	  Context c { memory, registers, input, output };
	  Opcode::testCode(c.code());
	  Opcode::Map[c.code()](c);
	}
    }

    std::vector<int> memory;
    Queue& input;
    Queue& output;
    Registers registers;
  };
}
