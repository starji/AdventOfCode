
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

template<int Param>
int input(std::vector<int>& memory, opcode op, int ip)
{
  return op[Param] != 0 ? memory.at(ip + Param) : memory.at(memory.at(ip + Param));
}

template<int Param>
int& output(std::vector<int>& memory, opcode op, int ip)
{
  return memory.at(memory.at(ip + Param));
}

int step(std::vector<int>& memory, int index)
{
  auto op = separate(memory.at(index));
  switch(op[0])
    {
    case 1:
      {
	output<3>(memory, op, index) = input<1>(memory, op, index) + input<2>(memory, op, index);
	return index + 4;
      }
      break;
    case 2:
      {
	output<3>(memory, op, index) = input<1>(memory, op, index) * input<2>(memory, op, index);
	return index + 4;
      }
      break;
    case 3:
      {
	std::cout << "Enter Value:";
	std::cin >> output<3>(memory, op, index);
	return index + 2;
      }
      break;
    case 4:
      {
	std::cout << input<1>(memory, op, index) << std::endl;
	return index + 2;
      }
      break;

    case 5:
      {
	if (input<1>(memory, op, index) != 0)
	  {
	    return input<2>(memory, op, index);
	  }
	else
	  {
	    return index + 3;
	  }
      }
      break;
    case 6:
      {
	if (input<1>(memory, op, index) == 0)
	  {
	    return input<2>(memory, op, index);;
	  }
	else
	  {
	    return index + 3;
	  }
      }
      break;
    case 7:
      {
	output<3>(memory, op, index) = input<1>(memory, op, index) < input<2>(memory, op, index) ? 1 : 0;

	return index + 4;
      }
      break;
    case 8:
      {
	output<3>(memory, op, index) = input<1>(memory, op, index) == input<2>(memory, op, index) ? 1 : 0;
	return index + 4;
      }
      break;
    default:
      throw std::runtime_error("unknown opcode");
    }
}

void run (std::vector<int> buffer)
{
  int index = 0;
  while (buffer.at(index) != 99)
    {
      index = step(buffer, index);
    }
}
