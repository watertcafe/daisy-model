// action.h -- Manager actions
// 
// Copyright 1996-2001 Per Abrahamsen and S�ren Hansen
// Copyright 2000-2001 KVL.
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


#ifndef ACTION_H
#define ACTION_H

#include "librarian.h"
#include "alist.h"

class Daisy;

class Action : public Model
{
  // Content.
public:
  const symbol name;
  const AttributeList alist;

  // Simulation.
public:
  virtual void tick (const Daisy&, Treelog&);
  virtual void doIt (Daisy&, Treelog&) = 0;
  virtual bool done (const Daisy&, Treelog&) const;
  virtual void output (Log&) const;

  // Create and Destroy.
public: 
  virtual bool check (const Daisy&, Treelog& err) const;
  static const char *const description;
private:
  Action (const Action&);
  Action& operator= (const Action&);
protected:
  explicit Action (Block&, const AttributeList& al);
  explicit Action (Block& al);
public:
  ~Action ();
};

#ifdef FORWARD_TEMPLATES
template<>
BuildBase* Librarian<Action>::content;
#endif

static Librarian<Action> Action_init ("action");

#endif // ACTION_H
