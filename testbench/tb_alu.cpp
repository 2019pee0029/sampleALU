#include <systemc.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "../include/alu.h"

using namespace std;

// Comprehensive testbench for ALU (Arithmetic Logic Unit)
// Operations: 0=ADD, 1=SUBTRACT, 2=AND, 3=OR, 4=XOR, 5=SHL, 6=SHR, 7=Reserved
SC_MODULE(testbench_alu)
{
    // 8-bit ALU
    alu<8>* alu_8bit;
    sc_signal<int8_t>  sig_a_8, sig_b_8, sig_result_8;
    sc_signal<sc_uint<3>> sig_op_8;
    sc_signal<bool>    sig_zf_8, sig_sf_8, sig_ov_8, sig_uf_8, sig_cf_8;
    
    // 16-bit ALU
    alu<16>* alu_16bit;
    sc_signal<int16_t> sig_a_16, sig_b_16, sig_result_16;
    sc_signal<sc_uint<3>> sig_op_16;
    sc_signal<bool>    sig_zf_16, sig_sf_16, sig_ov_16, sig_uf_16, sig_cf_16;
    
    // 32-bit ALU
    alu<32>* alu_32bit;
    sc_signal<int32_t> sig_a_32, sig_b_32, sig_result_32;
    sc_signal<sc_uint<3>> sig_op_32;
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
        cout << "\n" << string(100, '=') << endl;
        cout << "ALU (Arithmetic Logic Unit) Comprehensive Testbench" << endl;
        cout << "Operations: 0=ADD, 1=SUBTRACT, 2=AND, 3=OR, 4=XOR, 5=SHL, 6=SHR, 7=Reserved" << endl;
        cout << string(100, '=') << endl;
        cout.flush();
        
        wait(1, SC_NS);  // Allow initial Delta cycle
        
        // Test 8-bit ALU - Arithmetic Operations
        test_8bit_arithmetic();
        cout.flush();
        
        // Test 8-bit ALU - Logic Operations
        test_8bit_logic();
        cout.flush();
        
        // Test 8-bit ALU - Shift Operations
        test_8bit_shift();
        cout.flush();
        
        // Test 16-bit ALU - Arithmetic Operations
        test_16bit_arithmetic();
        cout.flush();
        
        // Test 16-bit ALU - Logic Operations
        test_16bit_logic();
        cout.flush();
        
        // Test 32-bit ALU - Arithmetic Operations
        test_32bit_arithmetic();
        cout.flush();
        
        // Test boundary conditions and flag behavior
        test_boundary_conditions_and_flags();
        cout.flush();
        
        cout << "\n" << string(100, '=') << endl;
        cout << "All ALU Comprehensive Tests Completed" << endl;
        cout << string(90, '=') << endl;
        cout.flush();
        
        sc_stop();
    }
    
    void test_8bit_arithmetic()
    {
        cout << "\n[TEST 1.1] 8-bit ALU - Arithmetic Operations (ADD/SUBTRACT)" << endl;
        cout << "Range: -128 to 127" << endl;
        cout << string(100, '-') << endl;
        cout << "A    | B    | Op | Result | Expected | ZF | SF | OV | UF | CF | Description" << endl;
        cout << string(100, '-') << endl;
        
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
            {100, 30, 0, 127, false, false, true, false, true, "ADD: Positive overflow"},
            {-100, -30, 0, -128, false, true, false, true, true, "ADD: Negative underflow"},
            {50, -50, 0, 0, true, false, false, false, false, "ADD: Result to zero"},
            {1, 1, 0, 2, false, false, false, false, false, "ADD: Small values"},
            {127, 1, 0, 127, false, false, true, false, true, "ADD: Max+1 overflow"},
            {-128, -1, 0, -128, false, true, false, true, true, "ADD: Min-1 underflow"},
            
            // SUBTRACT operations (op = 1)
            {10, 5, 1, 5, false, false, false, false, false, "SUB: Normal positive"},
            {-10, 5, 1, -15, false, true, false, false, false, "SUB: Normal negative"},
            {-100, 30, 1, -128, false, true, false, true, true, "SUB: Negative underflow"},
            {100, -30, 1, 127, false, false, true, false, true, "SUB: Positive overflow"},
            {50, 50, 1, 0, true, false, false, false, false, "SUB: Result to zero"},
            {-50, -50, 1, 0, true, false, false, false, false, "SUB: Negative to zero"},
            {0, 0, 1, 0, true, false, false, false, false, "SUB: Zero from zero"},
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
            
            string op_str = (test.op == 0) ? "ADD" : "SUB";
            cout << setw(4) << static_cast<int>(test.a) << " | "
                 << setw(4) << static_cast<int>(test.b) << " | "
                 << setw(2) << op_str << " | "
                 << setw(6) << static_cast<int>(result) << " | "
                 << setw(8) << static_cast<int>(test.expected_result) << " | "
                 << (zf ? "1" : "0") << "  | "
                 << (sf ? "1" : "0") << "  | "
                 << (ov ? "1" : "0") << "  | "
                 << (uf ? "1" : "0") << "  | "
                 << (cf ? "1" : "0") << "  | "
                 << (pass ? "PASS" : "FAIL") << " | " << test.description << endl;
        }
        
        cout << "\n8-bit Arithmetic: " << pass_count << "/" << test_count << " tests passed" << endl;
        cout.flush();
    }
    
    void test_8bit_logic()
    {
        cout << "\n[TEST 1.2] 8-bit ALU - Logic Operations (AND/OR/XOR)" << endl;
        cout << "Range: -128 to 127" << endl;
        cout << string(100, '-') << endl;
        cout << "A    | B    | Op  | Result | Expected | ZF | SF | Description" << endl;
        cout << string(100, '-') << endl;
        
        int pass_count = 0;
        int test_count = 0;
        
        struct TestCase {
            int8_t a, b;
            int op;
            int8_t expected_result;
            bool exp_zf, exp_sf;
            const char* op_str;
            const char* description;
        } tests[] = {
            // AND operations (op = 2)
            {15, 7, 2, 7, false, false, "AND", "AND: Basic positive"},
            {-1, 127, 2, 127, false, false, "AND", "AND: -1 with max positive"},
            {0, -1, 2, 0, true, false, "AND", "AND: 0 clears all bits"},
            {-1, -1, 2, -1, false, true, "AND", "AND: -1 with -1 gives -1"},
            {85, -86, 2, 0, true, false, "AND", "AND: Complementary patterns"},
            
            // OR operations (op = 3)
            {8, 4, 3, 12, false, false, "OR", "OR: Basic positive"},
            {1, 2, 3, 3, false, false, "OR", "OR: Single bits"},
            {0, 5, 3, 5, false, false, "OR", "OR: 0 with value"},
            {-1, 0, 3, -1, false, true, "OR", "OR: -1 sets all bits"},
            {64, 32, 3, 96, false, false, "OR", "OR: Power of two"},
            
            // XOR operations (op = 4)
            {15, 7, 4, 8, false, false, "XOR", "XOR: Basic positive"},
            {-1, 0, 4, -1, false, true, "XOR", "XOR: -1 flips all bits"},
            {5, 5, 4, 0, true, false, "XOR", "XOR: Same values give zero"},
            {85, -86, 4, -1, false, true, "XOR", "XOR: Complementary gives -1"},
            {0, 0, 4, 0, true, false, "XOR", "XOR: Zero with zero"},
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
            
            bool result_ok = (result == test.expected_result);
            bool zf_ok = (zf == test.exp_zf);
            bool sf_ok = (sf == test.exp_sf);
            
            bool pass = result_ok && zf_ok && sf_ok;
            if (pass) pass_count++;
            test_count++;
            
            cout << setw(4) << static_cast<int>(test.a) << " | "
                 << setw(4) << static_cast<int>(test.b) << " | "
                 << setw(3) << test.op_str << " | "
                 << setw(6) << static_cast<int>(result) << " | "
                 << setw(8) << static_cast<int>(test.expected_result) << " | "
                 << (zf ? "1" : "0") << "  | "
                 << (sf ? "1" : "0") << "  | "
                 << (pass ? "PASS" : "FAIL") << " | " << test.description << endl;
        }
        
        cout << "\n8-bit Logic: " << pass_count << "/" << test_count << " tests passed" << endl;
        cout.flush();
    }
    
    void test_8bit_shift()
    {
        cout << "\n[TEST 1.3] 8-bit ALU - Shift Operations (SHL/SHR)" << endl;
        cout << "Range: -128 to 127, Shift limited to 31 positions" << endl;
        cout << string(100, '-') << endl;
        cout << "A    | B (Shift) | Op  | Result | Expected | ZF | SF | Description" << endl;
        cout << string(100, '-') << endl;
        
        int pass_count = 0;
        int test_count = 0;
        
        struct TestCase {
            int8_t a, b;
            int op;
            int8_t expected_result;
            bool exp_zf, exp_sf;
            const char* op_str;
            const char* description;
        } tests[] = {
            // LEFT SHIFT operations (op = 5)
            {1, 1, 5, 2, false, false, "SHL", "SHL: 1 << 1 = 2"},
            {2, 2, 5, 8, false, false, "SHL", "SHL: 2 << 2 = 8"},
            {4, 3, 5, 32, false, false, "SHL", "SHL: 4 << 3 = 32"},
            {1, 0, 5, 1, false, false, "SHL", "SHL: Shift by 0"},
            {16, 3, 5, -128, false, true, "SHL", "SHL: 16 << 3 = 128 (wraps to -128)"},
            
            // RIGHT SHIFT operations (op = 6, arithmetic)
            {8, 1, 6, 4, false, false, "SHR", "SHR: 8 >> 1 = 4"},
            {16, 2, 6, 4, false, false, "SHR", "SHR: 16 >> 2 = 4"},
            {127, 1, 6, 63, false, false, "SHR", "SHR: 127 >> 1 = 63"},
            {-8, 1, 6, -4, false, true, "SHR", "SHR: -8 >> 1 = -4 (arithmetic)"},
            {-1, 1, 6, -1, false, true, "SHR", "SHR: -1 >> 1 = -1 (all 1s)"},
            {1, 0, 6, 1, false, false, "SHR", "SHR: Shift by 0"},
            {0, 4, 6, 0, true, false, "SHR", "SHR: Zero shifted"},
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
            
            bool result_ok = (result == test.expected_result);
            bool zf_ok = (zf == test.exp_zf);
            bool sf_ok = (sf == test.exp_sf);
            
            bool pass = result_ok && zf_ok && sf_ok;
            if (pass) pass_count++;
            test_count++;
            
            cout << setw(4) << static_cast<int>(test.a) << " | "
                 << setw(9) << static_cast<int>(test.b) << " | "
                 << setw(3) << test.op_str << " | "
                 << setw(6) << static_cast<int>(result) << " | "
                 << setw(8) << static_cast<int>(test.expected_result) << " | "
                 << (zf ? "1" : "0") << "  | "
                 << (sf ? "1" : "0") << "  | "
                 << (pass ? "PASS" : "FAIL") << " | " << test.description << endl;
        }
        
        cout << "\n8-bit Shift: " << pass_count << "/" << test_count << " tests passed" << endl;
        cout.flush();
    }
    
    void test_16bit_arithmetic()
    {
        cout << "\n[TEST 2.1] 16-bit ALU - Arithmetic Operations (ADD/SUBTRACT)" << endl;
        cout << "Range: -32768 to 32767" << endl;
        cout << string(100, '-') << endl;
        cout << "A      | B      | Op  | Result  | Expected | ZF | SF | OV | UF | CF | Description" << endl;
        cout << string(100, '-') << endl;
        
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
            {30000, 5000, 0, 32767, false, false, true, false, true, "ADD: Positive overflow"},
            {-30000, -5000, 0, -32768, false, true, false, true, true, "ADD: Negative underflow"},
            {100, -100, 0, 0, true, false, false, false, false, "ADD: Result to zero"},
            
            // SUBTRACT operations
            {3000, 1000, 1, 2000, false, false, false, false, false, "SUB: Normal positive"},
            {-30000, 5000, 1, -32768, false, true, false, true, true, "SUB: Negative underflow"},
            {30000, -5000, 1, 32767, false, false, true, false, true, "SUB: Positive overflow"},
            {0, 0, 1, 0, true, false, false, false, false, "SUB: Zero from zero"},
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
            
            string op_str = (test.op == 0) ? "ADD" : "SUB";
            cout << setw(6) << static_cast<int>(test.a) << " | "
                 << setw(6) << static_cast<int>(test.b) << " | "
                 << setw(3) << op_str << " | "
                 << setw(7) << static_cast<int>(result) << " | "
                 << setw(8) << static_cast<int>(test.expected_result) << " | "
                 << (zf ? "1" : "0") << "  | "
                 << (sf ? "1" : "0") << "  | "
                 << (ov ? "1" : "0") << "  | "
                 << (uf ? "1" : "0") << "  | "
                 << (cf ? "1" : "0") << "  | "
                 << (pass ? "PASS" : "FAIL") << " | " << test.description << endl;
        }
        
        cout << "\n16-bit Arithmetic: " << pass_count << "/" << test_count << " tests passed" << endl;
        cout.flush();
    }
    
    void test_16bit_logic()
    {
        cout << "\n[TEST 2.2] 16-bit ALU - Logic Operations (AND/OR/XOR)" << endl;
        cout << "Range: -32768 to 32767" << endl;
        cout << string(100, '-') << endl;
        cout << "A      | B      | Op  | Result  | Expected | ZF | SF | Description" << endl;
        cout << string(100, '-') << endl;
        
        int pass_count = 0;
        int test_count = 0;
        
        struct TestCase {
            int16_t a, b;
            int op;
            int16_t expected_result;
            bool exp_zf, exp_sf;
            const char* op_str;
            const char* description;
        } tests[] = {
            // AND operations (op = 2)
            {255, 127, 2, 127, false, false, "AND", "AND: Basic positive"},
            {-1, 32767, 2, 32767, false, false, "AND", "AND: -1 with max positive"},
            
            // OR operations (op = 3)
            {256, 128, 3, 384, false, false, "OR", "OR: Basic positive"},
            {-1, 0, 3, -1, false, true, "OR", "OR: -1 sets all bits"},
            
            // XOR operations (op = 4)
            {1000, 2000, 4, 1080, false, false, "XOR", "XOR: Basic positive"},
            {-1, 0, 4, -1, false, true, "XOR", "XOR: -1 flips all bits"},
            {0, 0, 4, 0, true, false, "XOR", "XOR: Zero with zero"},
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
            
            bool result_ok = (result == test.expected_result);
            bool zf_ok = (zf == test.exp_zf);
            bool sf_ok = (sf == test.exp_sf);
            
            bool pass = result_ok && zf_ok && sf_ok;
            if (pass) pass_count++;
            test_count++;
            
            cout << setw(6) << static_cast<int>(test.a) << " | "
                 << setw(6) << static_cast<int>(test.b) << " | "
                 << setw(3) << test.op_str << " | "
                 << setw(7) << static_cast<int>(result) << " | "
                 << setw(8) << static_cast<int>(test.expected_result) << " | "
                 << (zf ? "1" : "0") << "  | "
                 << (sf ? "1" : "0") << "  | "
                 << (pass ? "PASS" : "FAIL") << " | " << test.description << endl;
        }
        
        cout << "\n16-bit Logic: " << pass_count << "/" << test_count << " tests passed" << endl;
        cout.flush();
    }
    
    void test_32bit_arithmetic()
    {
        cout << "\n[TEST 3.1] 32-bit ALU - Arithmetic Operations (ADD/SUBTRACT)" << endl;
        cout << "Range: -2147483648 to 2147483647" << endl;
        cout << string(100, '-') << endl;
        cout << "A         | B         | Op  | Result      | Expected    | ZF | SF | Description" << endl;
        cout << string(100, '-') << endl;
        
        int pass_count = 0;
        int test_count = 0;
        
        struct TestCase {
            int32_t a, b;
            int op;
            int32_t expected_result;
            bool exp_zf, exp_sf;
            const char* description;
        } tests[] = {
            // ADD operations
            {1000000, 2000000, 0, 3000000, false, false, "ADD: Normal positive"},
            {-1000000, -2000000, 0, -3000000, false, true, "ADD: Normal negative"},
            {100000, -100000, 0, 0, true, false, "ADD: Result to zero"},
            
            // SUBTRACT operations
            {1000000, 500000, 1, 500000, false, false, "SUB: Normal positive"},
            {-1000000, 1000000, 1, -2000000, false, true, "SUB: Normal negative"},
            {0, 0, 1, 0, true, false, "SUB: Zero from zero"},
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
            
            bool result_ok = (result == test.expected_result);
            bool zf_ok = (zf == test.exp_zf);
            bool sf_ok = (sf == test.exp_sf);
            
            bool pass = result_ok && zf_ok && sf_ok;
            if (pass) pass_count++;
            test_count++;
            
            string op_str = (test.op == 0) ? "ADD" : "SUB";
            cout << setw(9) << static_cast<int>(test.a) << " | "
                 << setw(9) << static_cast<int>(test.b) << " | "
                 << setw(3) << op_str << " | "
                 << setw(11) << static_cast<int>(result) << " | "
                 << setw(11) << static_cast<int>(test.expected_result) << " | "
                 << (zf ? "1" : "0") << "  | "
                 << (sf ? "1" : "0") << "  | "
                 << (pass ? "PASS" : "FAIL") << " | " << test.description << endl;
        }
        
        cout << "\n32-bit Arithmetic: " << pass_count << "/" << test_count << " tests passed" << endl;
        cout.flush();
    }
    
    void test_boundary_conditions_and_flags()
    {
        cout << "\n[TEST 4] Flag Behavior and Boundary Conditions - Cross-Operation" << endl;
        cout << string(100, '-') << endl;
        
        // Test Zero Flag across different operations
        cout << "\nZero Flag Tests (8-bit):" << endl;
        cout << "  • ADD (10 + (-10)):" << endl;
        sig_a_8.write(10);
        sig_b_8.write(-10);
        sig_op_8.write(0);  // ADD
        wait(1, SC_NS);
        cout << "    Result=" << static_cast<int>(sig_result_8.read()) 
             << ", ZF=" << (sig_zf_8.read() ? "SET" : "CLEAR") << endl;
        
        cout << "  • SUB (50 - 50):" << endl;
        sig_a_8.write(50);
        sig_b_8.write(50);
        sig_op_8.write(1);  // SUBTRACT
        wait(1, SC_NS);
        cout << "    Result=" << static_cast<int>(sig_result_8.read()) 
             << ", ZF=" << (sig_zf_8.read() ? "SET" : "CLEAR") << endl;
        
        cout << "  • AND (0 & 255):" << endl;
        sig_a_8.write(0);
        sig_b_8.write(255);
        sig_op_8.write(2);  // AND
        wait(1, SC_NS);
        cout << "    Result=" << static_cast<int>(sig_result_8.read()) 
             << ", ZF=" << (sig_zf_8.read() ? "SET" : "CLEAR") << endl;
        
        cout << "  • XOR (5 ^ 5):" << endl;
        sig_a_8.write(5);
        sig_b_8.write(5);
        sig_op_8.write(4);  // XOR
        wait(1, SC_NS);
        cout << "    Result=" << static_cast<int>(sig_result_8.read()) 
             << ", ZF=" << (sig_zf_8.read() ? "SET" : "CLEAR") << endl;
        
        cout << "  • SHR (0 >> 4):" << endl;
        sig_a_8.write(0);
        sig_b_8.write(4);
        sig_op_8.write(6);  // RIGHT SHIFT
        wait(1, SC_NS);
        cout << "    Result=" << static_cast<int>(sig_result_8.read()) 
             << ", ZF=" << (sig_zf_8.read() ? "SET" : "CLEAR") << endl;
        
        // Test Sign Flag across operations
        cout << "\nSign Flag Tests (8-bit):" << endl;
        cout << "  • ADD (-50 + (-40)):" << endl;
        sig_a_8.write(-50);
        sig_b_8.write(-40);
        sig_op_8.write(0);  // ADD
        wait(1, SC_NS);
        cout << "    Result=" << static_cast<int>(sig_result_8.read()) 
             << ", SF=" << (sig_sf_8.read() ? "SET" : "CLEAR") << endl;
        
        cout << "  • OR (-1 | 0):" << endl;
        sig_a_8.write(-1);
        sig_b_8.write(0);
        sig_op_8.write(3);  // OR
        wait(1, SC_NS);
        cout << "    Result=" << static_cast<int>(sig_result_8.read()) 
             << ", SF=" << (sig_sf_8.read() ? "SET" : "CLEAR") << endl;
        
        cout << "  • SHR (-8 >> 1) [Arithmetic]:" << endl;
        sig_a_8.write(-8);
        sig_b_8.write(1);
        sig_op_8.write(6);  // RIGHT SHIFT
        wait(1, SC_NS);
        cout << "    Result=" << static_cast<int>(sig_result_8.read()) 
             << ", SF=" << (sig_sf_8.read() ? "SET" : "CLEAR") << endl;
        
        // Test Overflow/Underflow (only for ADD/SUB)
        cout << "\nOverflow/Underflow Tests (8-bit, Arithmetic Only):" << endl;
        cout << "  • ADD (100 + 30) [Should saturate to 127]:" << endl;
        sig_a_8.write(100);
        sig_b_8.write(30);
        sig_op_8.write(0);  // ADD
        wait(1, SC_NS);
        cout << "    Result=" << static_cast<int>(sig_result_8.read()) 
             << ", OV=" << (sig_ov_8.read() ? "SET" : "CLEAR")
             << ", CF=" << (sig_cf_8.read() ? "SET" : "CLEAR") << endl;
        
        cout << "  • SUB (-100 - 30) [Should saturate to -128]:" << endl;
        sig_a_8.write(-100);
        sig_b_8.write(30);
        sig_op_8.write(1);  // SUBTRACT
        wait(1, SC_NS);
        cout << "    Result=" << static_cast<int>(sig_result_8.read()) 
             << ", UF=" << (sig_uf_8.read() ? "SET" : "CLEAR")
             << ", CF=" << (sig_cf_8.read() ? "SET" : "CLEAR") << endl;
        
        // Test Reserved operation (should default to ADD)
        cout << "\nReserved Operation Tests (8-bit, Op=7 should ADD):" << endl;
        sig_a_8.write(5);
        sig_b_8.write(3);
        sig_op_8.write(7);  // Reserved (should ADD)
        wait(1, SC_NS);
        cout << "  • 5 + 3 (Op=7 reserved) = " << static_cast<int>(sig_result_8.read()) << endl;
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
