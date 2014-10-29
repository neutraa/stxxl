/***************************************************************************
 *  include/stxxl/bits/common/custom_stats.h
 *
 *  Part of the STXXL. See http://stxxl.sourceforge.net
 *
 *  Copyright (C) 2014 Thomas Keh <thomas.keh@student.kit.edu>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************/

#ifndef STXXL_COMMON_CUSTOM_STATS_HEADER
#define STXXL_COMMON_CUSTOM_STATS_HEADER

#include <string>
#include <utility>
#include <algorithm>
#include <stxxl/bits/common/timer.h>
#include <stxxl/bits/common/utils.h>

STXXL_BEGIN_NAMESPACE

//! This class provides a statistical counter that can easily be
//! deactivated using a typedef to dummy_custom_stats_counter.
//! It's basically a wrapper for a unsigned long long value.
//!
//! \see dummy_custom_stats_counter
//! \see custom_stats_timer
class custom_stats_counter
{
    //! The counter type
    typedef unsigned long long counter_type;
    //! The counter's value
    counter_type val;
public:
    //! The constructor. Initializes the counter to 0.
    custom_stats_counter() : val(0) { }
    //! Increases the counter by right.
    //! \param right The corresponding integer value
    custom_stats_counter& operator+=(counter_type right)
    {
        val += right;
        return *this;
    }
    //! Increases the counter by 1 (prefix).
    custom_stats_counter& operator++()
    {
        ++val;
        return *this;
    }
    //! Increases the counter by 1 (postfix).
    custom_stats_counter& operator++(int)
    {
        custom_stats_counter * result = this;
        ++val;
        return *result;
    }
    //! Set the counter to other
    //! \param other The corresponding integer value
    custom_stats_counter& operator=(counter_type other)
    {
        val = other;
        return *this;
    }
    //! Set the counter to other if other is larger than the current counter value.
    //! \param other The corresponding integer value
    void set_max(counter_type other)
    {
        val = std::max(val,other);
    }
    //! Return the counter value interpreted as a memory amount in IEC units as a string.
    //! For that purpose the counter value is multiplied with the byte_per_element argument.
    //! \param byte_per_element The memory amount per "counter element".
    std::string as_memory_amount(counter_type byte_per_element) const
    {
        return format_IEC_size(val*byte_per_element) + "B";
    }
    //! Cast to counter_type: Returns the counter's value as a regular integer value.
    //! This can be used as a getter as well as for printing with std::out.
    operator counter_type() const
    {
        return val;
    }
};


//! This class provides a statistical timer that can easily be
//! deactivated using a typedef to dummy_custom_stats_timer.
//! It's basically a wrapper for stxxl::timer.
//!
//! \see dummy_custom_stats_timer
//! \see custom_stats_counter
class custom_stats_timer
{
    stxxl::timer timer;
public:
    //! Constructor. This does not start the timer!
    custom_stats_timer() : timer(false) { }
    //! Starts the timer.
    void start()
    {
        timer.start();
    }
    //! Stops the timer.
    void stop()
    {
        timer.stop();
    }
    //! <<-operator for std::ostream. Can be used for printing with std::cout.
    friend std::ostream& operator<<(std::ostream& os, const custom_stats_timer& o)
    {
        return os << o.timer.seconds() << " s";
    }
};

//! Dummy class for custom_stats_counter. The methods do nothing.
//! The compiler should optimize out the code.
//!
//! \see custom_stats_counter
//! \see dummy_custom_stats_timer
class dummy_custom_stats_counter
{
    typedef unsigned long long counter_type;
public:
    dummy_custom_stats_counter() {}
    dummy_custom_stats_counter& operator+=(counter_type) {
        return *this;
    }
    dummy_custom_stats_counter& operator++() {
        return *this;
    }
    dummy_custom_stats_counter& operator++(int) {
        return *this;
    }
    dummy_custom_stats_counter& operator=(counter_type) {
        return *this;
    }
    void set_max(counter_type) {}
    std::string as_memory_amount(counter_type) {
        return "";
    }
    operator counter_type() const
    {
        return counter_type();
    }
};

//! Dummy class for custom_stats_timer. The methods do nothing.
//! The compiler should optimize out the code.
//!
//! \see custom_stats_timer
//! \see dummy_custom_stats_counter
class dummy_custom_stats_timer
{
public:
    dummy_custom_stats_timer() { }
    void start() {}
    void stop() {}
    friend std::ostream& operator<<(std::ostream& os, const dummy_custom_stats_timer&)
    {
        return os;
    }
};

STXXL_END_NAMESPACE
#endif // !STXXL_COMMON_CUSTOM_STATS_HEADER