/******************************************************************************\
 *           ___        __                                                    *
 *          /\_ \    __/\ \                                                   *
 *          \//\ \  /\_\ \ \____    ___   _____   _____      __               *
 *            \ \ \ \/\ \ \ '__`\  /'___\/\ '__`\/\ '__`\  /'__`\             *
 *             \_\ \_\ \ \ \ \L\ \/\ \__/\ \ \L\ \ \ \L\ \/\ \L\.\_           *
 *             /\____\\ \_\ \_,__/\ \____\\ \ ,__/\ \ ,__/\ \__/.\_\          *
 *             \/____/ \/_/\/___/  \/____/ \ \ \/  \ \ \/  \/__/\/_/          *
 *                                          \ \_\   \ \_\                     *
 *                                           \/_/    \/_/                     *
 *                                                                            *
 * Copyright (C) 2011-2013                                                    *
 * Dominik Charousset <dominik.charousset@haw-hamburg.de>                     *
 *                                                                            *
 * This file is part of libcppa.                                              *
 * libcppa is free software: you can redistribute it and/or modify it under   *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation; either version 2.1 of the License,               *
 * or (at your option) any later version.                                     *
 *                                                                            *
 * libcppa is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
 * See the GNU Lesser General Public License for more details.                *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with libcppa. If not, see <http://www.gnu.org/licenses/>.            *
\******************************************************************************/


#ifndef CPPA_EXIT_REASON_HPP
#define CPPA_EXIT_REASON_HPP

#include <cstdint>

namespace cppa { namespace exit_reason {

/**
 * @brief Indicates that the actor is still alive.
 */
static constexpr std::uint32_t not_exited = 0x00000;

/**
 * @brief Indicates that an actor finished execution.
 */
static constexpr std::uint32_t normal = 0x00001;

/**
 * @brief Indicates that an actor finished execution
 *        because of an unhandled exception.
 */
static constexpr std::uint32_t unhandled_exception = 0x00002;

/**
 * @brief Indicates that an event-based actor
 *        tried to use {@link receive()} or a strongly typed actor tried
 *        to call {@link become()}.
 */
static constexpr std::uint32_t unallowed_function_call = 0x00003;

/**
 * @brief Indicates that the actor received an unexpected
 *        synchronous reply message.
 */
static constexpr std::uint32_t unhandled_sync_failure = 0x00004;

/**
 * @brief Indicates that a synchronous message timed out.
 */
static constexpr std::uint32_t unhandled_sync_timeout = 0x00005;

/**
 * @brief Indicates that the actor was forced to shutdown by
 *        a user-generated event.
 */
static constexpr std::uint32_t user_shutdown = 0x00010;

/**
 * @brief Indicates that an actor finishied execution
 *        because a connection to a remote link was
 *        closed unexpectedly.
 */
static constexpr std::uint32_t remote_link_unreachable = 0x00101;

/**
 * @brief Any user defined exit reason should have a
 *        value greater or equal to prevent collisions
 *        with default defined exit reasons.
 */
static constexpr std::uint32_t user_defined = 0x10000;

const char* as_string(std::uint32_t value);

} } // namespace cppa::exit_reason

#endif // CPPA_EXIT_REASON_HPP
