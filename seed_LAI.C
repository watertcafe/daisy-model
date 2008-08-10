// seed_LAI.C -- Initial growth is governed by a forced LAI function.
// 
// Copyright 2008 Per Abrahamsen and KU.
//
// This file is part of Daisy.
// 
// Daisy is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
// 
// Daisy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser Public License for more details.
// 
// You should have received a copy of the GNU Lesser Public License
// along with Daisy; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#define BUILD_DLL
#include "seed.h"
#include "block.h"
#include "alist.h"
#include "syntax.h"
#include "librarian.h"
#include "plf.h"
#include "log.h"
#include <cmath>

struct SeedLAI : public Seed
{
  // Paramaters.
  const double DSLAI05;		// DS at CAI=0.5; forced development
  const PLF& SpLAIfac;		// Factor defining max Specific leaf weight

  // Variables.
  bool InitCAI;			// Initial CAI development.

  // Simulation.
  double forced_CAI (double WLeaf, double SpLAI, double DS);
  double release_C (double dt)
  { return 0.0; }
  void output (Log& log) const
  { output_variable (InitCAI, log); }
  
  // Create and Destroy.
  double initial_N (const double /* weight */) const
  { return -42.42e42; }
  void initialize (const double /* weight */)
  { }
  bool check (Treelog&) const
  { return true; }
  static void load_syntax (Syntax& syntax, AttributeList& alist);
  SeedLAI (Block& al)
    : Seed (al),
      DSLAI05 (al.number ("DSLAI05")),
      SpLAIfac (al.plf ("SpLAIfac")),
      InitCAI (al.flag ("InitCAI"))
  { }
  ~SeedLAI ()
  { }
};

double
SeedLAI::forced_CAI (const double WLeaf, const double SpLAI, const double DS)
{
  if (!InitCAI)
    // No force.
    return -1.0;

  // This is the fixed curve for used right after emergence, when
  // there is not yet any significant leaf dry matter.
  const double CAI_fixed = 1.0/(1.0+std::exp(-15.0*(DS-DSLAI05)));

  // The is the maximal CAI we will allow in the initialization phase.
  double CAI_max;
  if (DS < 0.07)
    {
      // At the very start (when we have no leaf dry matter), we put
      // no limit on the CAI.  I.e. we use the fixed function as it.
      CAI_max = 10.;
    }
  else
    {
      // After that, we use SpLAIfac, which tell how much thiner the
      // early leafs may be.  We always allow a LAI of 0.01.
      CAI_max = std::max( 0.01, SpLAIfac (DS) * SpLAI * WLeaf);
    }

  // This is then our initial CAI esstimate.
  const double CAI_init = std::min (CAI_max, CAI_fixed);


  // If CAI_init is below CAI_exit, we will exit initialization phase.
  // The idea is that enough leaf DM has been generated to account for
  // the LAI using the ordinary mechanism, so we no longer need the
  // fixed initialization curve.
  const double CAI_exit = SpLAI * WLeaf;
  if (CAI_exit < CAI_init)
    return CAI_init;

  // Leaf mass is now large enough to be self-sustainable (hopefully).
  InitCAI = false;
  return -1.0;              // No force.
}

void
SeedLAI::load_syntax (Syntax& syntax, AttributeList& alist)
{
  // Parameters.
  syntax.add ("DSLAI05", Syntax::None (), Syntax::Const,
	      "DS at CAI=0.5; initial phase.");
  alist.add ("DSLAI05", 0.15);
  syntax.add ("SpLAIfac", "DS", Syntax::None (), Syntax::Const, "\
Factor defining maximum specific leaf weight.\n\
Only used during the initial phase.");
  PLF SpLf;
  SpLf.add (0.00, 3.00);
  SpLf.add (0.20, 1.50);
  SpLf.add (0.40, 1.25);
  SpLf.add (0.60, 1.00);
  alist.add ("SpLAIfac", SpLf);

  // State.
  syntax.add ("InitCAI", Syntax::Boolean, Syntax::State,
	      "Initial CAI development phase.");
  alist.add ("InitCAI", true);

}

const AttributeList& 
Seed::default_model ()
{
  static AttributeList alist;
  
  if (!alist.check ("type"))
    {
      Syntax dummy;
      SeedLAI::load_syntax (dummy, alist);
      alist.add ("type", "LAI");
    }
  return alist;
}

static struct Seed_LAISyntax
{
  static Model& make (Block& al)
  { return *new SeedLAI (al); }
  Seed_LAISyntax ()
  {
    Syntax& syntax = *new Syntax ();
    AttributeList& alist = *new AttributeList ();

    alist.add ("description", "\
Initial crop growth is governed by a forced LAI function.");
    SeedLAI::load_syntax (syntax, alist);

    Librarian::add_type (Seed::component, "LAI", alist, syntax, &make);
  }
} SeedLAI_syntax;

// seed_LAI.C ends here.