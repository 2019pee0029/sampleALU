#ifndef ALU_H
#define ALU_H

#include <systemc.h>
#include <cstdint>
#include <limits>

// Arithmetic Logic Unit (ALU) - Parametrized template supporting multiple operations
// Template parameter: BitWidth (8, 16, or 32)
// Operations: 
//   - 0: ADD
//   - 1: SUBTRACT
//   - 2: AND (bitwise)
//   - 3: OR (bitwise)
//   - 4: XOR (bitwise)
//   - 5: LEFT SHIFT
//   - 6: RIGHT SHIFT (arithmetic for signed)
//   - 7: Reserved

template <int BitWidth>
SC_MODULE(alu)
{
    // Determine the appropriate data type based on BitWidth
    using DataType = typename std::conditional<
        BitWidth == 8, int8_t,
        typename std::conditional<BitWidth == 16, int16_t, int32_t>::type
    >::type;

    // Input ports
    sc_in<DataType>      operand_a;     // First operand
    sc_in<DataType>      operand_b;     // Second operand / Shift amount
    sc_in<sc_uint<3>>    operation;     // Operation select: 0=ADD, 1=SUB, 2=AND, 3=OR, 4=XOR, 5=SHL, 6=SHR, 7=Reserved
    
    // Output ports
    sc_out<DataType>     result;         // Result (saturated if overflow/underflow)
    sc_out<bool>         zero_flag;      // High when result is zero
    sc_out<bool>         sign_flag;      // High when result is negative
    sc_out<bool>         overflow_flag;  // High when result exceeds max value
    sc_out<bool>         underflow_flag; // High when result is below min value
    sc_out<bool>         carry_flag;     // High when unsigned overflow occurs
    
    // Constructor
    SC_CTOR(alu)
    {
        SC_METHOD(compute_alu);
        sensitive << operand_a << operand_b << operation;
    }
    
    // Process: combinational logic for ALU operations
    void compute_alu()
    {
        DataType a_val = operand_a.read();
        DataType b_val = operand_b.read();
        sc_uint<3> op = operation.read();
        
        // Get the range for this data type
        constexpr DataType MIN_VAL = std::numeric_limits<DataType>::min();
        constexpr DataType MAX_VAL = std::numeric_limits<DataType>::max();
        
        // Cast to larger type to detect overflow/underflow
        int64_t result_extended = 0;
        bool ov = false;
        bool uf = false;
        bool is_arithmetic = false;  // Flag for overflow-sensitive operations
        
        // Perform selected operation
        switch(op)
        {
            case 0: // ADD
                is_arithmetic = true;
                result_extended = static_cast<int64_t>(a_val) + static_cast<int64_t>(b_val);
                break;
            case 1: // SUBTRACT
                is_arithmetic = true;
                result_extended = static_cast<int64_t>(a_val) - static_cast<int64_t>(b_val);
                break;
            case 2: // AND (bitwise)
                result_extended = static_cast<int64_t>(a_val) & static_cast<int64_t>(b_val);
                break;
            case 3: // OR (bitwise)
                result_extended = static_cast<int64_t>(a_val) | static_cast<int64_t>(b_val);
                break;
            case 4: // XOR (bitwise)
                result_extended = static_cast<int64_t>(a_val) ^ static_cast<int64_t>(b_val);
                break;
            case 5: // LEFT SHIFT
                result_extended = static_cast<int64_t>(a_val) << (b_val & 0x1F);  // Limit shift amount
                break;
            case 6: // RIGHT SHIFT (arithmetic for signed)
                result_extended = static_cast<int64_t>(a_val) >> (b_val & 0x1F);  // Limit shift amount
                break;
            case 7: // Reserved - default to ADD
            default:
                is_arithmetic = true;
                result_extended = static_cast<int64_t>(a_val) + static_cast<int64_t>(b_val);
                break;
        }
        
        // Check for overflow/underflow conditions (only for arithmetic operations)
        if (is_arithmetic)
        {
            ov = (result_extended > MAX_VAL);
            uf = (result_extended < MIN_VAL);
        }
        
        // Saturate result if overflow/underflow detected
        DataType saturated_result;
        if (ov)
        {
            saturated_result = MAX_VAL;
        }
        else if (uf)
        {
            saturated_result = MIN_VAL;
        }
        else
        {
            saturated_result = static_cast<DataType>(result_extended);
        }
        
        // Compute flags
        bool zero = (saturated_result == 0);
        bool sign = (saturated_result < 0);
        bool carry = (ov || uf);  // Carry flag set on any arithmetic exception
        
        // Write outputs
        result.write(saturated_result);
        zero_flag.write(zero);
        sign_flag.write(sign);
        overflow_flag.write(ov);
        underflow_flag.write(uf);
        carry_flag.write(carry);
    }
};

#endif // ALU_H
