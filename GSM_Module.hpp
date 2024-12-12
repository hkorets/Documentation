/*
* GSM_MODULE.hpp
 *
 *  Created on: Nov 2, 2024
 *      Author: gllekk
 */

#ifndef INC_GSM_MODULE_HPP_
#define INC_GSM_MODULE_HPP_

extern "C"{
#include "main.h"
}

#include <string>

/**
 * @brief Loads parameters for initializing the GSM module.
 * @return Parameters structure containing UART handle and GPIO pin details.
 */
struct Parameters{
    UART_HandleTypeDef *uart_handle;

    uint16_t rx_pin;
    GPIO_TypeDef *rx_port;

    uint16_t tx_pin;
    GPIO_TypeDef *tx_port;
};

/**
* @class GSMModule
*
* A module to control a GSM module with an STM32 microcontroller,
* enabling seamless communication. The library will
* serve as the basis for building a basic smartphone
* that utilizes the GSM module’s communication features.
*/
class GSM_Module{

    enum State{
        IDLE,
        CALLING,
        RECEIVE_CALL,
        RINGING,
        HANG_UP,
        SEND_SMS,
        RECEIVE_SMS,
        UNKNOWN
    };

public:

    const char END_OF_MSG = 0x1A;

    /**
     * @brief Constructor for the GSM module.
     * @param parameters Initialization parameters including UART handle and GPIO pins.
     */
    GSM_Module(const Parameters& parameters);

     /**
    * @brief Sends an SMS to the specified number with the given message.
    * @param number Recipient's phone number.
    * @param message SMS message content.
    */
    void send_sms(const char* number, const char* message);
    /**
     * @brief Initiates a call to the specified phone number.
     * @param number Phone number to call.
     */
    void make_call(const char* number);
    /**
    * @brief Hangs up the current call.
    */
    void hang_up();

    /**
    * @brief Recieves an incoming call.
    */
    void receive_call();
    /**
    * @brief Recieves an incoming SMS.
    */
    void receive_sms();
     /**
    * @brief Transmits data via UART.
    * @param data Pointer to the data buffer.
    * @param size Size of the data buffer.
    * @return True if transmission is successful, false otherwise.
    */
    bool transmit(const char* data, size_t size);
     /**
    * @brief Receives data via UART.
    * @param buffer Pointer to the buffer for storing received data.
    * @param size Maximum size of the buffer.
    * @return True if reception is successful, false otherwise.
    */
    bool receive(char* buffer, size_t size);
      /**
     * @brief Starts receiving data via UART interrupts.
     */
    void start_receiving();
    /**
     * @brief Retrieves the signal strength of the GSM connection.
     * @return Signal strength as an integer value.
     */
    int get_signal_strength();
    /**
    * @brief Retrieves the current date and time from the GSM module.
    * @return A pair containing the date and time as strings.
    */
    std::pair<std::string, std::string> GSM_Module::get_date();


    State prev_state = IDLE; ///< The previous state of the GSM module.
    State current_state = IDLE; ///< The current state of the GSM module.
    /**
     * @brief Handles the response for signal strength queries.
     * @param buffer Buffer containing the response string.
     */
    void receive_signal_strength(std::string buffer);
    /**
    * @brief Receives the date and time from the GSM module and processes it.
    * @param buffer The buffer containing the date and time data.
    */
    void receive_date_and_time(std::string buffer);

private:

    static constexpr const char* MSG = "AT+CMGS="; ///< AT command to send SMS.
    static constexpr const char* MSG_TEXT_MODE = "AT+CMGF=1\r\n"; ///< AT command to set SMS text mode.

    static constexpr const char* CALL = "ATD+"; ///< AT command to make a call.


    static constexpr const char* AT = "AT\r\n"; ///< AT command to check GSM module status.

    static constexpr const char* GPS_ON = "AT+GPS=1\r\n"; ///< AT command to enable GPS.
    static constexpr const char* GPS_OFF = "AT+GPS=0\r\n"; ///< AT command to disable GPS.
    static constexpr const char* GET_TIME = "AT+CCLK?\r\n"; ///< AT command to get time.
    static constexpr const char* GET_SIGNAL = "AT+CSQ\r\n"; ///< AT command to get signal strength.

    uint16_t rx_index = 0; ///< Index for the receive buffer.
    uint8_t rx_buffer[256]; ///< Buffer to store received data.
    Parameters parameters; ///< Parameters to configure the GSM module.
      /**
     * @brief Sends an AT command to the GSM module and verifies the response.
     * @param command Command string to be sent.
     * @return True if the command execution is successful, false otherwise.
     */
    bool send_at_command(const char* command);
    void read_sms(int index);
      /**
     * @brief Handles UART interruption and processes incoming data.
     */
    void handle_interruption();
      /**
     * @brief UART receive complete callback function for handling data.
     * @param huart Pointer to the UART handle.
     */
    friend void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);

    int signal = 0;
    std::string time;
    std::string date;

};

[[maybe_unused]] static GSM_Module* gsm = nullptr;

Parameters load_parameters();

#endif /* INC_GSM_MODULE_HPP_ */