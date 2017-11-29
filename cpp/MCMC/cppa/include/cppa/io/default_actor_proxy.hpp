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


#ifndef DEFAULT_ACTOR_PROXY_HPP
#define DEFAULT_ACTOR_PROXY_HPP

#include "cppa/extend.hpp"
#include "cppa/actor_proxy.hpp"
#include "cppa/memory_cached.hpp"
#include "cppa/io/default_protocol.hpp"
#include "cppa/intrusive/single_reader_queue.hpp"

namespace cppa { namespace detail {

class memory;
class instance_wrapper;
template<typename>
class basic_memory_cache;

} } // namespace cppa::detail

namespace cppa { namespace io {

class sync_request_info : public extend<memory_managed>::with<memory_cached> {

    friend class detail::memory;

 public:

    typedef sync_request_info* pointer;

    pointer      next;   // intrusive next pointer
    actor_ptr    sender; // points to the sender of the message
    message_id mid;    // sync message ID

 private:

    sync_request_info(actor_ptr sptr, message_id id);

};

class default_actor_proxy : public actor_proxy {

    typedef actor_proxy super;

 public:

    default_actor_proxy(actor_id mid,
                        const process_information_ptr& pinfo,
                        default_protocol* parent);

    void enqueue(const message_header& hdr, any_tuple msg) override;

    void link_to(const actor_ptr& other) override;

    void unlink_from(const actor_ptr& other) override;

    bool remove_backlink(const actor_ptr& to) override;

    bool establish_backlink(const actor_ptr& to) override;

    void local_link_to(const actor_ptr& other) override;

    void local_unlink_from(const actor_ptr& other) override;

    void deliver(const message_header& hdr, any_tuple msg) override;

    inline const process_information_ptr& process_info() const {
        return m_pinf;
    }

 protected:

    ~default_actor_proxy();

 private:

    void forward_msg(const message_header& hdr, any_tuple msg);

    default_protocol*       m_parent;
    process_information_ptr m_pinf;
    intrusive::single_reader_queue<sync_request_info, detail::disposer> m_pending_requests;

};

} } // namespace cppa::network

#endif // DEFAULT_ACTOR_PROXY_HPP
