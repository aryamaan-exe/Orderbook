#include "Simulation.hpp"

using Builder = Simulation::Builder;

Builder& Builder::AddOrderbooks(const std::vector<Orderbook>& books) {
  if (type_ == SimulationType::Basic) {
    throw std::logic_error("Basic (hardcoded) simulation cannot be modified.");
  }

  books_ = books;
}

Builder& Builder::AddCommands(const std::vector<Command>& commands) {
  if (type_ == SimulationType::Basic) {
    throw std::logic_error("Basic (hardcoded) simulation cannot be modified.");
  }
  
  commands_ = commands;
}

Simulation Builder::Build() const {
  return Simulation{type_, commands_, books_};
}

Simulation::Simulation(SimulationType type, const std::vector<Command>& commands, const std::vector<Orderbook>& books): type_(type), commands_(commands), books_(books) {}