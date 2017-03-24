
	    /**
	     * A switch used to turn on/off exploitation dynamics
	     * (e.g turn off for virgin equilibrium)
	     */
	    bool exploitation_on = true;

	    /**
	     * A switch used to turn on/off the calculation
	     * of exploitation rates from catches. Used to specify a particular
	     * exploitation rate when calculation MSY/Bmsy
	     */
	    bool catches_on = true;

	    /**
	     * Vulnerable biomass by sector
	     */
	    Array<double,Sector> biomass_vulnerable = 0;

	    /**
	     * Catches by sector
	     */
	    Array<double,Sector> catches = 0;

	    Array<double,Sector> exploitation_rate_max = 1;

	    /**
	     * Exploitation rate by region and method for current time step
	     */
	    Array<double,Sector> exploitation_rate = 0;

	    /**
	     * Exploitation survival by sex and age
	     */
	    Array<double,Sex,Age> exploitation_survivals = 1;

