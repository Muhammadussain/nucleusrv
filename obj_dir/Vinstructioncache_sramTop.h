// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary model header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef VERILATED_VINSTRUCTIONCACHE_SRAMTOP_H_
#define VERILATED_VINSTRUCTIONCACHE_SRAMTOP_H_  // guard

#include "verilated.h"

class Vinstructioncache_sramTop__Syms;
class Vinstructioncache_sramTop___024root;

// This class is the main interface to the Verilated model
class Vinstructioncache_sramTop VL_NOT_FINAL : public VerilatedModel {
  private:
    // Symbol table holding complete model state (owned by this class)
    Vinstructioncache_sramTop__Syms* const vlSymsp;

  public:

    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(&clk_i,0,0);
    VL_IN8(&rst_i,0,0);
    VL_IN8(&csb_i,0,0);
    VL_IN8(&wmask_i,3,0);
    VL_IN8(&we_i,0,0);
    VL_IN16(&addr_i,12,0);
    VL_IN(&wdata_i,31,0);
    VL_OUT(&rdata_o,31,0);

    // CELLS
    // Public to allow access to /* verilator public */ items.
    // Otherwise the application code can consider these internals.

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    Vinstructioncache_sramTop___024root* const rootp;

    // CONSTRUCTORS
    /// Construct the model; called by application code
    /// If contextp is null, then the model will use the default global context
    /// If name is "", then makes a wrapper with a
    /// single model invisible with respect to DPI scope names.
    explicit Vinstructioncache_sramTop(VerilatedContext* contextp, const char* name = "TOP");
    explicit Vinstructioncache_sramTop(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    virtual ~Vinstructioncache_sramTop();
  private:
    VL_UNCOPYABLE(Vinstructioncache_sramTop);  ///< Copying not allowed

  public:
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    /// Retrieve name of this model instance (as passed to constructor).
    const char* name() const;

    // Abstract methods from VerilatedModel
    const char* hierName() const override final;
    const char* modelName() const override final;
    unsigned threads() const override final;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
