# Orderbook

A concurrent limit orderbook implementation in C++, supporting market, limit, and fill-or-kill order types.

## Design

Each orderbook is owned by a thread which processes that book's incoming orders sequentially.

Supported order types:
- **Market:** executes immediately against the best available price(s)
- **Limit:** executes at a specified price or better, otherwise rests on the book
- **Fill-or-Kill:** must be filled in its entirety immediately, or is cancelled outright

### In Progress: Simulations

You can also run simulations of the matching engine. There are three different kinds you can run: Basic, Random, and Custom.

The basic simulation is a hardcoded simulation that aims to test all order types across several orderbooks.

Random simulations are, as the name suggests, completely randomized, from order types to prices and quantities.

You can also define a custom simulation and add your own orders.


## Building & Running

Clone this repository and add the two [Catch2 amalgamated files](https://github.com/catchorg/Catch2/tree/devel/extras) 
into a directory called `catch2`.

```bash
make exec && ./bin/exec    # run the orderbook
make tests && ./bin/tests  # run the test suite
```