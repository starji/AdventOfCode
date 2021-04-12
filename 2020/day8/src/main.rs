use std::str::FromStr;
use std::num::ParseIntError;
use std::collections::HashSet;

#[derive(Debug, PartialEq, Clone)]
enum Operation {
    Acc,
    Jmp,
    Nop
}

#[derive(Debug, PartialEq, Clone)]
struct Instruction {
    operation: Operation,
    argument: i32
}

impl FromStr for Instruction {
    type Err = ParseIntError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut parts = s.split(' ');

        let operation = match parts.next().unwrap() {
            "acc" => Operation::Acc,
            "jmp" => Operation::Jmp,
            "nop" => Operation::Nop,
            _ => Operation::Nop
        };
        let argument = parts.next().unwrap().parse()?;

        Ok(Instruction { operation, argument })
    }
}


fn part1_run(instructions: &Vec<Instruction>) -> (bool, i32) {
    let mut visited = HashSet::new ();

    let mut ip: i32 = 0;
    let mut acc = 0;

    while visited.get(&ip).is_none() && (ip as usize) < instructions.len() {
        visited.insert(ip);
        match instructions[ip as usize].operation {
            Operation::Acc => {
                acc = acc + instructions[ip as usize].argument;
                ip = ip + 1;
            }
            Operation::Jmp => {
                ip = ip + instructions[ip as usize].argument;
            }
            Operation::Nop => {
                ip = ip + 1;
            }
        }
    }

    return ((ip as usize) >= instructions.len(), acc);
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let instructions: Vec<Instruction> = contents.trim().lines().map(|x| Instruction::from_str(x).unwrap()).collect();

    let mut i = 0;
    while i < instructions.len() {
        match instructions[i].operation {
            Operation::Acc => { }
            Operation::Jmp => {
                let mut instruction_copy = instructions.clone();
                instruction_copy[i].operation = Operation::Nop;

                let (finished, acc) = part1_run(&instruction_copy);
                if finished {
                    println!("Found jmp-to-nop: {} acc={}", i, acc);
                    break;
                }
            }
            Operation::Nop => {
                let mut instruction_copy = instructions.clone();
                instruction_copy[i].operation = Operation::Jmp;

                let (finished, acc) = part1_run(&instruction_copy);
                if finished {
                    println!("Found nop-to-jmp: {} acc={}", i, acc);
                    break;
                }
            }
        }

        i = i + 1;
    }

//    println!("{:?}", part1_run(&instructions));

}
