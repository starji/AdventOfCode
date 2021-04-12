using opcode = std::array<int, 4>;
opcode separate(int value)
{
  return { (value % 100),
	   ((value / 100) % 10),
	   ((value / 1000) % 10),
	   ((value / 10000) % 10) };
}
class IntcodeMachine
{
public:
  IntcodeMachine(std::vector<int> memory)
    : memory(memory)
  {
    setIP(0);
  }

  void setIP(int nextIP)
  {
    ip = nextIP;
    op = separate(memory.at(ip));
  }

  void run()
  {
    while(op[0] != 99)
      {
	setIP(step());
      }
  }

  int step()
  {
    switch(op[0])
      {
      case 1:
	{
	  write<3>(read<1>() + read<2>());
	  return ip + 4;
	}
	break;
      case 2:
	{
	  write<3>(read<1>() * read<2>());
	  return ip + 4;
	}
	break;
      case 3:
	{
	  std::cout << "Enter Value:";
	  std::cin >> writable<1>();
	  return ip + 2;
	}
	break;
      case 4:
	{
	  std::cout << read<1>() << std::endl;
	  return ip + 2;
	}
	break;

      case 5:
	{
	  if (read<1>() != 0)
	    {
	      return read<2>();
	    }
	  else
	    {
	      return ip + 3;
	    }
	}
	break;
      case 6:
	{
	  if (read<1>() == 0)
	    {
	      return read<2>();
	    }
	  else
	    {
	      return ip + 3;
	    }
	}
	break;
      case 7:
	{
	  write<3>(read<1>() < read<2>() ? 1 : 0);
	  return ip + 4;
	}
	break;
      case 8:
	{
	  write<3>(read<1>() == read<2>() ? 1 : 0);
	  return ip + 4;
	}
	break;
      default:
	throw std::runtime_error("unknown opcode");
      }
  }

  template<int Param>
  int read()
  {
    return op[Param] != 0 ? memory.at(ip + Param) : memory.at(memory.at(ip + Param));
  }

  template<int Param>
  int& writable()
  {
    return memory.at(memory.at(ip + Param));
  }

  template<int Param>
  void write(int value)
  {
    memory.at(memory.at(ip + Param)) = value;
  }

  // Registers
  int ip {};

  // Current opcode
  opcode op {};

  // System memory
  std::vector<int> memory;
};
