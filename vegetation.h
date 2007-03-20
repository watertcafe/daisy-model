// vegetation.h
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


#ifndef VEGETATION_H
#define VEGETATION_H

#include "librarian.h"
#include <vector>

class Time;
class Weather;
class Bioclimate;
class Geometry;
class Soil;
class OrganicMatter;
class SoilHeat;
class SoilWater;
class SoilNH4;
class SoilNO3;
class PLF;
class Harvest;
class AM;

class Vegetation : public Model
{ 
  // Content.
public:
  const symbol name;
  static const char *const description;
  const double EpInterchange_;  // Soil to canopy pot-.evap. interchange.

  // Canopy queries.
public:
  virtual double shared_light_fraction () const;
  virtual double rs_min () const = 0; // Minimum transpiration resistance.
  virtual double rs_max () const = 0; // Maximum transpiration resistance.
  virtual double LAI () const = 0; // Total LAI of all crops [0-]
  virtual double height () const = 0; // Max crop height in canopy [cm]
  virtual double cover () const = 0; // Fraction of soil covered by crops [0-1]
  virtual const PLF& LAIvsH () const = 0; // LAI below height [f: cm -> R]
  virtual const PLF& HvsLAI () const = 0; // Height with LAI below [f: R -> cm]
  virtual double ACExt () const = 0;	// Canopy extinction coefficient
  virtual double ACRef () const = 0;	// Canopy reflection coefficient 
  virtual double ARExt () const = 0;	// Radiation Extinction coefficient
  virtual double EpFactor () const = 0;	// Reference to pot. evapotransp
  double EpInterchange () const; // Soil to canopy exchange rate.
  virtual double albedo () const = 0;	// Another reflection factor
  virtual double interception_capacity () const = 0;// Canopy water cap. [mm]

  // Individual crop queries.
public:
  virtual double DS_by_name (symbol name) const = 0;// [-1:2]/DSremove
  virtual double DM_by_name (symbol name, double height) const = 0;// Shoot DM [kg/ha]
  virtual std::string crop_names () const = 0;

  // Simulation
public:
  virtual void tick (const Time&, double relative_humidity,
                     const Bioclimate&, const Geometry&, const Soil&,
		     OrganicMatter&, const SoilHeat&, const SoilWater&,
		     // Allow plants to grow (hourly).
		     SoilNH4&, SoilNO3&, 
		     double& residuals_DM,
		     double& residuals_N_top, double& residuals_C_top,
		     std::vector<double>& residuals_N_soil,
		     std::vector<double>& residuals_C_soil,
                     double dt, Treelog&) = 0;
  virtual double transpiration (// Actual trans. [mm/h]
				double potential_transpiration,	
				double canopy_evaporation,
                                const Geometry& geo,
				const Soil& soil, SoilWater& soil_water,
				double day_fraction, double dt, Treelog&) = 0;
  virtual void force_production_stress  (double pstress) = 0;
  virtual void kill_all (symbol, const Time&, const Geometry&,
			 Bioclimate&, std::vector<AM*>& residuals, 
			 double& residuals_DM,
			 double& residuals_N_top, double& residuals_C_top,
			 std::vector<double>& residuals_N_soil,
			 std::vector<double>& residuals_C_soil,
			 Treelog&) = 0;
  virtual void emerge (symbol crop_name, Treelog&) = 0;
  virtual void harvest (symbol column_name,
			symbol crop_name,
			const Time&, const Geometry&, 
			Bioclimate& bioclimate,
			double stub_length,
			double stem_harvest,
			double leaf_harvest, 
			double sorg_harvest,
			std::vector<const Harvest*>& harvest,
                        double& min_height,
			std::vector<AM*>& residuals,
			double& harvest_DM, 
			double& harvest_N, double& harvest_C,
			double& residuals_DM,
			double& residuals_N_top, double& residuals_C_top,
			std::vector<double>& residuals_N_soil,
			std::vector<double>& residuals_C_soil,
                        const bool combine,
			Treelog&) = 0;
  virtual void sow (const AttributeList& al, 
                    const Geometry&, OrganicMatter&, 
                    double& seed_N /* kg/ha/h */,
                    double& seed_C /* kg/ha/h */,
                    const Time&, double dt, Treelog& msg) = 0;
  virtual void output (Log&) const;
  
  // Litter
  virtual double litter_vapor_flux_factor () const = 0;
  virtual double litter_water_capacity () const = 0;  
  virtual double litter_albedo () const = 0;  

  // Create and Destroy.
public:
  virtual void initialize (const Time&, const Geometry& geo,
                           const Soil& soil, OrganicMatter&,
                           Treelog&) = 0;
  static void load_syntax (Syntax&, AttributeList&);
  explicit Vegetation (Block&);
  ~Vegetation ();
};

#ifdef FORWARD_TEMPLATES
template<>
BuildBase* Librarian<Vegetation>::content;
#endif

static Librarian<Vegetation> Vegetation_init ("vegetation");

#endif // VEGETATION_H
