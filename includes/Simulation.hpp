#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <string>
#include <vector>
#include "CommandHandler.hpp"
#include "Orderbook.hpp"

enum class SimulationType { Basic, Random, Custom };

class Simulation {
public:
  class Builder {
  public:
    Builder(SimulationType type): type_(type) {}
    
    Builder& AddOrderbooks(const std::vector<Orderbook>& books);
    Builder& AddCommands(const std::vector<Command>& commands);
    
    Simulation Build() const;
  private:
    SimulationType type_;
    std::vector<Orderbook> books_;
    std::vector<Command> commands_;
  };

  Simulation(SimulationType type, const std::vector<Command>& commands, const std::vector<Orderbook>& books);

  void Run();
  void Benchmark();
private:
  SimulationType type_;
  std::vector<Command> commands_;
  CommandHandler cmdh_;
  std::vector<Orderbook> books_;
};

#endif