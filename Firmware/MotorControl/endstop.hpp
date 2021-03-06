#ifndef __ENDSTOP_HPP
#define __ENDSTOP_HPP

class Endstop {
   public:
    struct Config_t {
        float offset      = 0;
        float debounce_ms = 50.0f;
        uint16_t gpio_num;
        bool enabled        = false;
        bool is_active_high = false;
    };

    Endstop(Endstop::Config_t& config);

    Endstop::Config_t& config_;
    Axis* axis_ = nullptr;

    void update_endstop_config();
    void set_endstop_enabled(bool enable);

    void update();
    bool getEndstopState();

    bool endstop_state_ = false;

    auto make_protocol_definitions() {
        return make_protocol_member_list(
            make_protocol_ro_property("endstop_state", &endstop_state_),
            make_protocol_object("config",
                                 make_protocol_property("gpio_num", &config_.gpio_num,
                                                        [](void* ctx) { static_cast<Endstop*>(ctx)->update_endstop_config(); }, this),
                                 make_protocol_property("enabled", &config_.enabled,
                                                        [](void* ctx) { static_cast<Endstop*>(ctx)->update_endstop_config(); }, this),
                                 make_protocol_property("offset", &config_.offset),
                                 make_protocol_property("is_active_high", &config_.is_active_high),
                                 make_protocol_property("debounce_ms", &config_.debounce_ms)));
    }

   private:
    bool pin_state_                = false;
    float pos_when_pressed_        = 0.0f;
    volatile float debounce_timer_ = 0;
};
#endif