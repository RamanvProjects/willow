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


#ifndef CPPA_ACTOR_BEHAVIOR_HPP
#define CPPA_ACTOR_BEHAVIOR_HPP

#include <functional>

#include "cppa/config.hpp"
#include "cppa/extend.hpp"
#include "cppa/threadless.hpp"
#include "cppa/actor_state.hpp"
#include "cppa/local_actor.hpp"
#include "cppa/mailbox_based.hpp"
#include "cppa/mailbox_element.hpp"
#include "cppa/message_priority.hpp"

#include "cppa/detail/memory.hpp"
#include "cppa/detail/receive_policy.hpp"

#include "cppa/intrusive/single_reader_queue.hpp"

namespace cppa {

class scheduler;
namespace util { class fiber; }

enum class resume_result {
    actor_blocked,
    actor_done
};

enum scheduled_actor_type {
    context_switching_impl,  // enqueued to the job queue on startup
    event_based_impl,        // not enqueued to the job queue on startup
    default_event_based_impl // scheduler enqueues a 'RUN' message on startup
};

class scheduled_actor;

/**
 * @brief A base class for cooperatively scheduled actors.
 * @extends local_actor
 */
class scheduled_actor : public extend<local_actor>::with<mailbox_based, threadless> {

    typedef combined_type super;

 public:

    ~scheduled_actor();

    /**
     * @brief Intrusive next pointer needed by the scheduler's job queue.
     */
    scheduled_actor* next;

    /**
     * @brief Continues execution of this actor.
     * @param next_job Denotes the next actor to be resumed, should be
     *                 set in case of chaining.
     * @note This member function is called from the scheduler's worker threads.
     */
    virtual resume_result resume(util::fiber* from, actor_ptr& next_job) = 0;

    /**
     * @brief Called once by the scheduler after actor is initialized,
     *        but before @p resume is called.
     */
    void attach_to_scheduler(scheduler* sched, bool hidden);

    /**
     * @brief Returns @p false if this actor can be disposed immediately
     *        after construction, @p true otherwise.
     */
    virtual bool has_behavior() = 0;

    /**
     * @brief Denotes wheter this actor is enqueued to the scheduler's
     *        job queue on startup.
     */
    virtual scheduled_actor_type impl_type() = 0;

    /**
     * @brief Returns @p true if this actor is ignored by
     *        {@link await_all_others_done()}, false otherwise.
     */
    inline bool is_hidden() const;

    virtual void run_detached();

    void enqueue(const message_header&, any_tuple) override;

    bool chained_enqueue(const message_header&, any_tuple) override;

    void unchain() override;

 protected:

    scheduled_actor(actor_state init_state, bool enable_chained_send);

    void cleanup(std::uint32_t reason) override;

    typedef intrusive::single_reader_queue<mailbox_element, detail::disposer>
            mailbox_type;

    actor_state compare_exchange_state(actor_state expected, actor_state desired);

    inline void set_state(actor_state new_value) {
        m_state.store(new_value);
    }

    inline actor_state state() const {
        return m_state;
    }

    bool initialized() const;

 private:

    bool enqueue_impl(actor_state, const message_header&, any_tuple&&);

    std::atomic<actor_state> m_state;

 protected:

    scheduler* m_scheduler;
    bool m_hidden;

};

typedef intrusive_ptr<scheduled_actor> scheduled_actor_ptr;

inline bool scheduled_actor::is_hidden() const {
    return m_hidden;
}

} // namespace cppa

#endif // CPPA_ACTOR_BEHAVIOR_HPP
