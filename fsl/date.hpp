#pragma once

#include <boost/date_time/gregorian/gregorian.hpp>

namespace Fsl {

typedef boost::gregorian::date Date;

typedef boost::gregorian::days Days;
typedef boost::gregorian::weeks Weeks;
typedef boost::gregorian::months Months;
typedef boost::gregorian::years Years;

typedef boost::gregorian::year_iterator YearIterator;

} // end namespace Fsl
