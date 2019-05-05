#include "timer.hpp"

namespace chr {

chr_timer_t::chr_timer_t(uint32_t interval_ms, timer_callback_t &cb)
{
    m_trigger = channel_t<int>::create();
    m_cb = std::move(cb);
    m_interval_ms = interval_ms;
    LOG << "timer:" << this << " created\n";
}

chr_timer_t::~chr_timer_t()
{    
    LOG << "timer:" << this << " destroyed\n";
}


int chr_timer_t::select(int wait_ms)
{
    if (!m_running) {
        return 0;
    }
    
    // @wait_ms will be ignored, as 0
    return m_selecter.select_once();
}

bool chr_timer_t::start()
{
    if (m_running) {
        LOG << "chr_timer_t::start ignored: already running\n";
        return false;
    }

    if (m_cb == nullptr || m_interval_ms == 0) {
        LOG << "chr_timer_t::start error: m_cb == nullptr || m_interval_ms == 0\n";
        return false;
    }

    m_running = true;
    // start the trigger chroutine
    m_trigger_chroutine_id = ENGIN.create_son_chroutine([&](void *){
        while (m_running) {
            SLEEP(m_interval_ms);
            *m_trigger << 1;
        }     
        m_trigger_chroutine_id = INVALID_ID;
        LOG << "timer:" << this << " stopped!\n";
    }, nullptr);

    // add select case
    m_selecter.add_case(m_trigger.get(), &m_d, [&](){
        LOG << "timer triggled !!!" << std::endl;
        ENGIN.create_son_chroutine([&](void *){
            if (m_cb != nullptr) m_cb();
        }, nullptr);
    });

    return true;
}

void chr_timer_t::stop()
{
    if (!m_running) {
        LOG << "chr_timer_t::stop ignored: not running\n";
        return;
    }
    m_running = false;
    m_cb = nullptr;
    m_selecter.del_case(m_trigger.get());
    LOG << "timer:" << this << " stopping ...\n";
    while (m_trigger_chroutine_id != INVALID_ID) {
        SLEEP(5);
    }
    
}

}