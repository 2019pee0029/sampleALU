#include <systemc.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "../include/alu.h"

using namespace std;

// Simple testbench for ALU (Arithmetic Logic Unit) with ADD/SUBTRACT operations
SC_MODULE(testbench_alu)
{
    // 8-bit ALU
    alu<8>* alu_8bit;
    sc_signal<int8_t>  sig_a_8, sig_b_8, sig_result_8;
    sc_signal<sc_uint<2>> sig_op_8;
    sc_signal<bool>    sig_zf_8, sig_sf_8, sig_ov_8, sig_uf_8, sig_cf_8;
    
    // 16-bit ALU
    alu<16>* alu_16bit;
    sc_signal<int16_t> sig_a_16, sig_b_16, sig_result_16;
    sc_signal<sc_uint<2>> sig_op_16;
    sc_signal<bool>    sig_zf_16, sig_sf_16, sig_ov_16, sig_uf_16, sig_cf_16;
    
    // 32-bit ALU
    alu<32>* alu_32bit;
    sc_signal<int32_t> sig_a_32, sig_b_32, sig_result_32;
    sc_signal<sc_uint<2>> sig_op_32;
    sc_signal<bool>    sig_zf_32, sig_sf_32, sig_ov_32, sig_uf_32, sig_cf_32;
    
    SC_CTOR(testbench_alu)
    {
        // Instantiate 8-bit ALU
        alu_8bit = new alu<8>("alu_8bit");
        alu_8bit->operand_a(sig_a_8);
        alu_8bit->operand_b(sig_b_8);
        alu_8bit->operation(sig_op_8);
        alu_8bit->result(sig_result_8);
        alu_8bit->zero_flag(sig_zf_8);
        alu_8bit->sign_flag(sig_sf_8);
        alu_8bit->overflow_flag(sig_ov_8);
        alu_8bit->underflow_flag(sig_uf_8);
        alu_8bit->carry_flag(sig_cf_8);
        
        // Instantiate 16-bit ALU
        alu_16bit = new alu<16>("alu_16bit");
        alu_16bit->operand_a(sig_a_16);
        alu_16bit->operand_b(sig_b_16);
        alu_16bit->operation(sig_op_16);
        alu_16bit->result(sig_result_16);
        alu_16bit->zero_flag(sig_zf_16);
        alu_16bit->sign_flag(sig_sf_16);
        alu_16bit->overflow_flag(sig_ov_16);
        alu_16bit->underflow_flag(sig_uf_16);
        alu_16bit->carry_flag(sig_cf_16);
        
        // Instantiate 32-bit ALU
        alu_32bit = new alu<32>("alu_32bit");
        alu_32bit->operand_a(sig_a_32);
        alu_32bit->operand_b(sig_b_32);
        alu_32bit->operation(sig_op_32);
        alu_32bit->result(sig_result_32);
        alu_32bit->zero_flag(sig_zf_32);
        alu_32bit->sign_flag(sig_sf_32);
        alu_32bit->overflow_flag(sig_ov_32);
        alu_32bit->underflow_flag(sig_uf_32);
        alu_32bit->carry_flag(sig_cf_32);
        
        // Create thread for test vectors
        SC_THREAD(run_all_tests);
    }
    
    void run_all_tests()
    {
        cout << "\n" << string(90, '=') << endl;
        cout << "ALU (Arithmetic Logic Unit) Testbench" << endl;
        cout << "Operations: 0=ADD, 1=SUBTRACT, 2-3=Reserved (default to ADD)" << endl;
        cout << string(90, '=') << endl;
        cout.flush();
        
        wait(1, SC_NS);  // Allow initial Delta cycle
        
        // Test 8-bit ALU
        test_8bit_alu();
        cout.flush();
        
        // Test 16-bit ALU
        test_16bit_alu();
        cout.flush();
        
        // Test 32-bit ALU
        test_32bit_alu();
        cout.flush();
        
        // Test boundary conditions and flag behavior
        test_boundary_conditions_and_flags();
        cout.flush();
        
        cout << "\n" << string(90, '=') << endl;
        cout << "All ALU tests completed" << endl;
        cout << string(90, '=') << endl;
        cout.flush();
        
        sc_stop();
    }
    
    void test_8bit_alu()
    {
        cout << "\n[TEST 1] 8-bit ALU (Range: -128 to 127)" << endl;
        cout << string(90, '-') << endl;
        cout << "A    | B    | Op | Result | Expected | ZF | SF | OV | UF | CF | Status" << endl;
        cout << string(90, '-') << endl;
        
        int pass_count = 0;
        int test_count = 0;
        
        struct TestCase {
            int8_t a, b;
            int op;
            int8_t expected_result;
            bool exp_zf, exp_sf, exp_ov, exp_uf, exp_cf;
            const char* description;
        } tests[] = {
            // ADD operations (op = 0)
            {10, 5, 0, 15, false, false, false, false, false, "ADD: Normal positive"},
            {-10, -5, 0, -15, false, true, false, false, false, "ADD: Normal negative"},
            {100, 30, 0, 127, false, false, true, false, true, "ADD: Positive overflow (saturate)"},
            {-100, -30, 0, -128, false, true, false, true, true, "ADD: Negative underflow (saturate)"},
            {50, -50, 0, 0, true, false, false, false, false, "ADD: Result to zero"},
            
            // SUBTRACT operations (op = 1)
            {10, 5, 1, 5, false, false, false, false, false, "SUB: Normal positive"},
            {-10, 5, 1, -15, false, true, false, false, false, "SUB: Normal negative"},
            {-100, 30, 1, -128, false, true, false, true, true, "SUB: Negative underflow (saturate)"},
            {100, -30, 1, 127, false, false, true, false, true, "SUB: Positive overflow (saturate)"},
            {50, 50, 1, 0, true, false, false, false, false, "SUB: Result to zero"},
        };
        
        for (auto& test : tests)
        {
            sig_a_8.write(test.a);
            sig_b_8.write(test.b);
            sig_op_8.write(test.op);
            wait(1, SC_NS);
            
            int8_t result = sig_result_8.read();
            bool zf = sig_zf_8.read();
            bool sf = sig_sf_8.read();
            bool ov = sig_ov_8.read();
            bool uf = sig_uf_8.read();
            bool cf = sig_cf_8.read();
            
            bool result_ok = (result == test.expected_result);
            bool zf_ok = (zf == test.exp_zf);
            bool sf_ok = (sf == test.exp_sf);
            bool ov_ok = (ov == test.exp_ov);
            bool uf_ok = (uf == test.exp_uf);
            bool cf_ok = (cf == test.exp_cf);
            
            bool pass = result_ok && zf_ok && sf_ok && ov_ok && uf_ok && cf_ok;
            if (pass) pass_count++;
            test_count++;
            
            cout << setw(4) << static_cast<int>(test.a) << " | "
                 << setw(4) << static_cast<int>(test.b) << " | "
                 << setw(2) << test.op << " | "
                 << setw(6) << static_cast<int>(result) << " | "
                 << setw(8) << static_cast<int>(test.expected_result) << " | "
                 << (zf ? "1" : "0") << "  | "
                 << (sf ? "1" : "0") << "  | "
                 << (ov ? "1" : "0") << "  | "
                 << (uf ? "1" : "0") << "  | "
                 << (cf ? "1" : "0") << "  | "
                 << (pass ? "PASS" : "FAIL") << endl;
        }
        
        cout << "\n8-bit ALU: " << pass_count << "/" << test_count << " tests passed" << endl;
        cout.flush();
    }
    
    void test_16bit_alu()
    {
        cout << "\n[TEST 2] 16-bit ALU (Range: -32768 to 32767)" << endl;
        cout << string(90, '-') << endl;
        cout << "A      | B      | Op | Result  | Expected | ZF | SF | OV | UF | CF | Status" << endl;
        cout << string(90, '-') << endl;
        
        int pass_count = 0;
        int test_count = 0;
        
        struct TestCase {
            int16_t a, b;
            int op;
            int16_t expected_result;
            bool exp_zf, exp_sf, exp_ov, exp_uf, exp_cf;
            const char* description;
        } tests[] = {
            // ADD operations
            {1000, 2000, 0, 3000, false, false, false, false, false, "ADD: Normal positive"},
            {-1000, -2000, 0, -3000, false, true, false, false, false, "ADD: Normal negative"},
            {30000, 5000, 0, 32767, false, false, true, false, true, "ADD: Positive overflow (saturate)"},
            {-30000, -5000, 0, -32768, false, true, false, true, true, "ADD: Negative underflow (saturate)"},
            
            // SUBTRACT operations
            {3000, 1000, 1, 2000, false, false, false, false, false, "SUB: Normal positive"},
            {-30000, 5000, 1, -32768, false, true, false, true, true, "SUB: Negative underflow (saturate)"},
            {30000, -5000, 1, 32767, false, false, true, false, true, "SUB: Positive overflow (saturate)"},
        };
        
        for (auto& test : tests)
        {
            sig_a_16.write(test.a);
            sig_b_16.write(test.b);
            sig_op_16.write(test.op);
            wait(1, SC_NS);
            
            int16_t result = sig_result_16.read();
            bool zf = sig_zf_16.read();
            bool sf = sig_sf_16.read();
            bool ov = sig_ov_16.read();
            bool uf = sig_uf_16.read();
            bool cf = sig_cf_16.read();
            
            bool result_ok = (result == test.expected_result);
            bool zf_ok = (zf == test.exp_zf);
            bool sf_ok = (sf == test.exp_sf);
            bool ov_ok = (ov == test.exp_ov);
            bool uf_ok = (uf == test.exp_uf);
            bool cf_ok = (cf == test.exp_cf);
            
            bool pass = result_ok && zf_ok && sf_ok && ov_ok && uf_ok && cf_ok;
            if (pass) pass_count++;
            test_count++;
            
            cout << setw(6) << static_cast<int>(test.a) << " | "
                 << setw(6) << static_cast<int>(test.b) << " | "
                 << setw(2) << test.op << " | "
                 << setw(7) << static_cast<int>(result) << " | "
                 << setw(8) << static_cast<int>(test.expected_result) << " | "
                 << (zf ? "1" : "0") << "  | "
                 << (sf ? "1" : "0") << "  | "
                 << (ov ? "1" : "0") << "  | "
                 << (uf ? "1" : "0") << "  | "
                 << (cf ? "1" : "0") << "  | "
                 << (pass ? "PASS" : "FAIL") << endl;
        }
        
        cout << "\n16-bit ALU: " << pass_count << "/" << test_count << " tests passed" << endl;
        cout.flush();
    }
    
    void test_32bit_alu()
    {
        cout << "\n[TEST 3] 32-bit ALU (Range: -2147483648 to 2147483647)" << endl;
        cout << string(90, '-') << endl;
        cout << "A        | B        | Op | Result     | Expected   | ZF | SF | OV | UF | CF | Status" << endl;
        cout << string(90, '-') << endl;
        
        int pass_count = 0;
        int test_count = 0;
        
        struct TestCase {
            int32_t a, b;
            int op;
            int32_t expected_result;
            bool exp_zf, exp_sf, exp_ov, exp_uf, exp_cf;
            const char* description;
        } tests[] = {
            // ADD operations
            {1000000, 2000000, 0, 3000000, false, false, false, false, false, "ADD: Normal positive"},
            {-1000000, -2000000, 0, -3000000, false, true, false, false, false, "ADD: Normal negative"},
            
            // SUBTRACT operations
            {1000000, 500000, 1, 500000, false, false, false, false, false, "SUB: Normal positive"},
            {-1000000, 1000000, 1, -2000000, false, true, false, false, false, "SUB: Normal negative"},
        };
        
        for (auto& test : tests)
        {
            sig_a_32.write(test.a);
            sig_b_32.write(test.b);
            sig_op_32.write(test.op);
            wait(1, SC_NS);
            
            int32_t result = sig_result_32.read();
            bool zf = sig_zf_32.read();
            bool sf = sig_sf_32.read();
            bool ov = sig_ov_32.read();
            bool uf = sig_uf_32.read();
            bool cf = sig_cf_32.read();
            
            bool result_ok = (result == test.expected_result);
            bool zf_ok = (zf == test.exp_zf);
            bool sf_ok = (sf == test.exp_sf);
            bool ov_ok = (ov == test.exp_ov);
            bool uf_ok = (uf == test.exp_uf);
            bool cf_ok = (cf == test.exp_cf);
            
            bool pass = result_ok && zf_ok && sf_ok && ov_ok && uf_ok && cf_ok;
            if (pass) pass_count++;
            test_count++;
            
            cout << setw(8) << static_cast<int>(test.a) << " | "
                 << setw(8) << static_cast<int>(test.b) << " | "
                 << setw(2) << test.op << " | "
                 << setw(10) << static_cast<int>(result) << " | "
                 << setw(10) << static_cast<int>(test.expected_result) << " | "
                 << (zf ? "1" : "0") << "  | "
                 << (sf ? "1" : "0") << "  | "
                 << (ov ? "1" : "0") << "  | "
                 << (uf ? "1" : "0") << "  | "
                 << (cf ? "1" : "0") << "  | "
                 << (pass ? "PASS" : "FAIL") << endl;
        }
        
        cout << "\n32-bit ALU: " << pass_count << "/" << test_count << " tests passed" << endl;
        cout.flush();
    }
    
    void test_boundary_conditions_and_flags()
    {
        cout << "\n[TEST 4] Flag Behavior and Boundary Conditions" << endl;
        cout << string(90, '-') << endl;
        
        // Test Zero Flag
        cout << "\nZero Flag Test (8-bit):" << endl;
        sig_a_8.write(10);
        sig_b_8.write(10);
        sig_op_8.write(1);  // SUBTRACT
        wait(1, SC_NS);
        cout << "10 - 10 = " << static_cast<int>(sig_result_8.read()) 
             << ", Zero Flag: " << (sig_zf_8.read() ? "SET" : "CLEAR") << endl;
        
        // Test Sign Flag
        cout << "\nSign Flag Test (8-bit):" << endl;
        sig_a_8.write(5);
        sig_b_8.write(10);
        sig_op_8.write(1);  // SUBTRACT
        wait(1, SC_NS);
        cout << "5 - 10 = " << static_cast<int>(sig_result_8.read()) 
             << ", Sign Flag: " << (sig_sf_8.read() ? "SET" : "CLEAR") << endl;
        
        // Test Reserved operations (should default to ADD)
        cout << "\nReserved Operation Test (8-bit, Op=2 and Op=3 should ADD):" << endl;
        sig_a_8.write(5);
        sig_b_8.write(3);
        sig_op_8.write(2);  // Reserved (should ADD)
        wait(1, SC_NS);
        cout << "5 + 3 (Op=2 reserved) = " << static_cast<int>(sig_result_8.read()) << endl;
        
        sig_op_8.write(3);  // Reserved (should ADD)
        wait(1, SC_NS);
        cout << "5 + 3 (Op=3 reserved) = " << static_cast<int>(sig_result_8.read()) << endl;
        cout.flush();
    }
};

int sc_main(int argc, char* argv[])
{
    // Disable warning logging for cleaner output
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
    
    testbench_alu tb("testbench_alu");
    
    sc_start();
    
    return 0;
}
