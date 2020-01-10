#include <memory>
#include <iostream>
#include <vector>
#include <limits>
#include "gtest/gtest.h"
#include "tests/macros.h"
#include "lib/loader.h"
#include "lib/executor.h"
#include "lib/instantiator.h"

using std::make_unique;
using std::cout;
using std::endl;
using std::vector;
using std::numeric_limits;

/**
 * Case ISAs:
 * 
 * i32.eqz / i64.eqz 
 * i32.eq / i64.eq
 * i32.ne / i64.ne
 * i32.lt_s / i64.lt_s
 * i32.lt_u / i64.lt_u
 * i32.gt_s / i64.gt_s
 * i32.gt_u / i64.gt_u
 * i32.le_s / i64.le_s
 * i32.le_u / i64.le_u
 * i32.ge_s / i64.ge_s
 * i32.ge_u / i64.ge_u
 * f32.eq / f64.eq
 * f32.ne / f64.ne
 * f32.lt / f64.lt
 * f32.gt / f64.gt
 * f32.le / f64.le
 * f32.ge / f64.ge
 * i32.clz / i64.clz
 * i32.ctz / i64.ctz
 * i32.popcnt / i64.popcnt
 * i32.add / i64.add / f32.add / f64.add
 * i32.sub / i64.sub / f32.sub / f64.sub
 * i32.mul / i64.mul / f32.mul / f64.mul
 * i32.div_s / i64.div_s
 * i32.div_u / i64.div_u
 * i32.rem_s / i64.rem_s
 * i32.rem_u / i64.rem_u
 * i32.and / i64.and
 * i32.or / i64.or
 * i32.xor / i64.xor
 * f32.abs / f64.abs
 * f32.neg / f64.neg
 * f32.ceil / f64.ceil
 * f32.floor / f64.floor
 * f32.trunc / f64.trunc
 * f32.nearest / f64.nearest
 * f32.sqrt / f64.sqrt
 * f32.min / f64.min
 * f32.max / f64.max
 * f32.copysign / f64.copysign
 */

TEST(TWVM, Arithmetic) {
  const auto executor = make_unique<Executor>();

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result f32)
        (f32.floor (f32.const 123.9))
      ))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7d, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xa, 0x1, 0x8, 0, 0x43, 0xcd, 0xcc, 0xf7, 0x42, 0x8e, 0xb,
      })));
  EXPECT_EQ(123, executor->inspectRunningResult<float>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result f64)
        (f64.floor (f64.const 255.6))
      ))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7c, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xe, 0x1, 0xc, 0, 0x44, 0x33, 0x33, 0x33, 0x33, 0x33, 0xf3, 0x6f, 0x40,
        0x9c, 0xb,
      })));
  EXPECT_EQ(255, executor->inspectRunningResult<double>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i32)
        (i32.div_u (i32.const 4000)
          (i32.div_s (i32.const -3000)
            (i32.mul (i32.const 2)
              (i32.sub (i32.const 11) 
                (i32.add (i32.const 521) (i32.const 149))
              ))))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7f, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0x19, 0x1, 0x17, 0, 0x41, 0xa0, 0x1f, 0x41, 0xc8, 0x68, 0x41, 0x2, 0x41,
        0xb, 0x41, 0x89, 0x4, 0x41, 0x95, 0x1, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0xb,
      })));
  EXPECT_EQ(2000, executor->inspectRunningResult<uint32_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i64)
        (i64.div_u (i64.const 4000)
          (i64.div_s (i64.const 21474836470) 
            (i64.mul (i64.const 2)
              (i64.sub (i64.const 4294967294)  
                (i64.add (i64.const 2147483647) (i64.const 100))
              ))))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7e, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0x24, 0x1, 0x22, 0, 0x42, 0xa0, 0x1f, 0x42, 0xf6, 0xff, 0xff, 0xff, 0xcf,
        0, 0x42, 0x2, 0x42, 0xfe, 0xff, 0xff, 0xff, 0xf, 0x42, 0xff, 0xff, 0xff, 0xff, 0x7, 0x42,
        0xe4, 0, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0xb,
      })));
  EXPECT_EQ(800, executor->inspectRunningResult<uint64_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i32)
        (i32.xor (i32.const 20)
          (i32.or (i32.const 123)  
            (i32.and (i32.const 100) (i32.const 200))
          ))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7f, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0x12, 0x1, 0x10, 0, 0x41, 0x14, 0x41, 0xfb, 0, 0x41, 0xe4, 0, 0x41,
        0xc8, 0x1, 0x71, 0x72, 0x73, 0xb,
      })));
  EXPECT_EQ(111, executor->inspectRunningResult<int32_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i64)
        (i64.xor (i64.const 20)
          (i64.or (i64.const 123)  
            (i64.and (i64.const 4294967294) (i64.const 2147483647))
          ))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7e, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0x18, 0x1, 0x16, 0, 0x42, 0x14, 0x42, 0xfb, 0, 0x42, 0xfe, 0xff, 0xff,
        0xff, 0xf, 0x42, 0xff, 0xff, 0xff, 0xff, 0x7, 0x83, 0x84, 0x85, 0xb,
      })));
  EXPECT_EQ(static_cast<int64_t>(2147483627), executor->inspectRunningResult<int64_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result f32)
        (f32.ceil
          (f32.sqrt
            (f32.abs (f32.const 1000.123))
          ))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7d, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xc, 0x1, 0xa, 0, 0x43, 0xdf, 0x7, 0x7a, 0x44, 0x8b, 0x91, 0x8d, 0xb,
      })));
  EXPECT_EQ(32, executor->inspectRunningResult<float>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result f64)
        (f64.ceil
          (f64.sqrt
            (f64.abs (f64.const 14294967294.123))
          ))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7c, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0x10, 0x1, 0xe, 0, 0x44, 0xe7, 0xfb, 0xf0, 0x1f, 0x5f, 0xa0, 0xa, 0x42,
        0x99, 0x9f, 0x9b, 0xb,
      })));
  EXPECT_EQ(119562, executor->inspectRunningResult<double>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result f32)
        (f32.div
          (f32.convert_i32_s
            (i32.add
              (f32.le (f32.const 2.3) (f32.const 2.3))
              (f32.ne (f32.const 2.3) (f32.const 2.4))
            )
          )
          (f32.convert_i32_s
            (i32.mul
              (i32.add
                (f32.eq (f32.const 1000.123) (f32.const 1000.123))
                (f32.gt (f32.const 1001.123) (f32.const 1000.123))
              )
              (i32.add
                (f32.ge (f32.const 1.1) (f32.const 1.1))
                (f32.lt (f32.const 1.0) (f32.const 1.2))
              ))))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7d, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0x4d, 0x1, 0x4b, 0, 0x43, 0x33, 0x33, 0x13, 0x40, 0x43, 0x33, 0x33, 0x13,
        0x40, 0x5f, 0x43, 0x33, 0x33, 0x13, 0x40, 0x43, 0x9a, 0x99, 0x19, 0x40, 0x5c, 0x6a, 0xb2, 0x43,
        0xdf, 0x7, 0x7a, 0x44, 0x43, 0xdf, 0x7, 0x7a, 0x44, 0x5b, 0x43, 0xdf, 0x47, 0x7a, 0x44, 0x43,
        0xdf, 0x7, 0x7a, 0x44, 0x5e, 0x6a, 0x43, 0xcd, 0xcc, 0x8c, 0x3f, 0x43, 0xcd, 0xcc, 0x8c, 0x3f,
        0x60, 0x43, 0, 0, 0x80, 0x3f, 0x43, 0x9a, 0x99, 0x99, 0x3f, 0x5d, 0x6a, 0x6c, 0xb2, 0x95, 
        0xb,
      })));
  EXPECT_EQ(0.5, executor->inspectRunningResult<float>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result f32)
        (f32.trunc
          (f32.min
            (f32.max (f32.const 100.1)(f32.const 0)) 
            (f32.const 4.22342)
          ))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7d, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0x16, 0x1, 0x14, 0, 0x43, 0x33, 0x33, 0xc8, 0x42, 0x43, 0, 0, 0,
        0, 0x97, 0x43, 0x42, 0x26, 0x87, 0x40, 0x96, 0x8f, 0xb,
      })));
  EXPECT_EQ(4, executor->inspectRunningResult<float>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i32)
        (i32.rem_u (i32.const -1) (i32.const 100))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7f, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xa, 0x1, 0x8, 0, 0x41, 0x7f, 0x41, 0xe4, 0, 0x70, 0x0b,
      })));
  EXPECT_EQ(95, executor->inspectRunningResult<uint32_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i64)
        (i64.rem_u (i64.const 1234567) (i64.const 100))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7e, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xd, 0x1, 0xb, 0, 0x42, 0x87, 0xad, 0xcb, 0, 0x42, 0xe4, 0, 0x82, 0x0b,
      })));
  EXPECT_EQ(67, executor->inspectRunningResult<uint64_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i32)
        (i32.ctz (i32.const 10000000))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7f, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xa, 0x1, 0x8, 0, 0x41, 0x80, 0xad, 0xe2, 0x4, 0x68, 0x0b,
      })));
  EXPECT_EQ(7, executor->inspectRunningResult<int32_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i64)
        (i64.ctz (i64.const 10000000000000000000)))) 
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7e, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0x10, 0x1, 0xe, 0, 0x42, 0x80, 0x80, 0xa0, 0xcf, 0xc8, 0xe0, 0xc8, 0xe3,
        0x8a, 0x7f, 0x7a, 0x0b,
      })));
  EXPECT_EQ(19, executor->inspectRunningResult<int64_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i32)
        (i32.clz (i32.const 999999999))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7f, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xb, 0x1, 0x9, 0, 0x41, 0xff, 0x93, 0xeb, 0xdc, 0x3, 0x67, 0x0b,
      })));
  EXPECT_EQ(2, executor->inspectRunningResult<int32_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i64)
        (i64.clz (i64.const 9999999999999999999))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7e, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0x10, 0x1, 0xe, 0, 0x42, 0xff, 0xff, 0x9f, 0xcf, 0xc8, 0xe0, 0xc8, 0xe3,
        0x8a, 0x7f, 0x79, 0x0b,
      })));
  EXPECT_EQ(0, executor->inspectRunningResult<int64_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i32)
        (i32.trunc_f32_s
          (f32.neg (f32.const 123.123)))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7f, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xb, 0x1, 0x9, 0, 0x43, 0xfa, 0x3e, 0xf6, 0x42, 0x8c, 0xa8, 0x0b,
      })));
  EXPECT_EQ(-123, executor->inspectRunningResult<int32_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result f32)
        (f32.nearest (f32.const 123.123)))) 
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7d, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xa, 0x1, 0x8, 0, 0x43, 0xfa, 0x3e, 0xf6, 0x42, 0x90, 0x0b,
      })));
  EXPECT_EQ(123, executor->inspectRunningResult<float>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result f32)
        (f32.add
        (f32.copysign (f32.const 1.0) (f32.const 2.0)
        (f32.copysign (f32.const 1.0) (f32.const -2.0))))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7d, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0x1b, 0x1, 0x19, 0, 0x43, 0, 0, 0x80, 0x3f, 0x43, 0, 0, 0,
        0x40, 0x43, 0, 0, 0x80, 0x3f, 0x43, 0, 0, 0, 0xc0, 0x98, 0x98, 0x92, 0x0b,
      })));
  EXPECT_EQ(-1, executor->inspectRunningResult<float>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result i32)
        (i32.popcnt (i32.const 1234567890))))
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7f, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xb, 0x1, 0x9, 0, 0x41, 0xd2, 0x85, 0xd8, 0xcc, 0x4, 0x69, 0x0b,
      })));
  EXPECT_EQ(12, executor->inspectRunningResult<int32_t>());

  /**
    (module
      (memory $0 1)
      (export "main" (func $main))
      (func $main (; 1 ;) (result f32)
        (f32.demote_f64 (f64.const 123.123e100)))) 
   */
  executor->execute(
    Instantiator::instantiate(
      Loader::init(vector<uint8_t>{
        START_BYTES, 0x1, 0x5, 0x1, 0x60, 0, 0x1, 0x7d, 0x3,
        0x2, 0x1, 0, 0x5, 0x3, 0x1, 0, 0x1, 0x7, 0x8, 0x1, 0x4, 0x6d, 0x61, 0x69, 0x6e,
        0, 0, 0xa, 0xe, 0x1, 0xc, 0, 0x44, 0x43, 0xfb, 0x5c, 0x13, 0x4c, 0x97, 0x21, 0x55,
        0xb6, 0x0b,
      })));
  EXPECT_EQ(numeric_limits<float>::infinity(), executor->inspectRunningResult<float>());
}
