/* Created By Hamna Mohiuddin @hamnamohi as a Google Summer of Code 2024 Project.*/

package BabyKyber

import chisel3._
import chisel3.util._
import chisel3.experimental._
import caravan.bus.common.{AbstrRequest, AbstrResponse}

// *** GENERIC IO BUNDLE (Timer style) ***
class BabyKyber_IO[A <: AbstrRequest, B <: AbstrResponse](gen: A, gen1: B) extends Bundle {
    // Bus interconnect interfaces
    val req = Flipped(Decoupled(gen))
    val rsp = Decoupled(gen1)
    
    // Control inputs (same as before)
    val enable = Input(Bool())
    val key_enable = Input(Bool())
    val encryption_enable = Input(Bool())
    val decryption_enable = Input(Bool())
    
    // *** INTERRUPT OUTPUTS (Timer style) ***
    val cio_babykyber_intr_key = Output(Bool())        // Key generation interrupt
    val cio_babykyber_intr_encrypt = Output(Bool())    // Encryption interrupt  
    val cio_babykyber_intr_decrypt = Output(Bool())    // Decryption interrupt
}

// *** GENERIC BABYKYBER MODULE (Timer style) ***
class BabyKyberTop[A <: AbstrRequest, B <: AbstrResponse](gen: A, gen1: B) extends Module {
  val io = IO(new BabyKyber_IO(gen, gen1))

  val validReg = RegInit(false.B)
  io.rsp.valid := validReg
  io.req.ready := true.B

  val rdata = Wire(SInt(32.W))

  // Instantiate the accelerator (BlackBox)
  val bkyber = Module(new kybertop)

  // Connect clock and reset
  bkyber.io.clk := clock.asUInt()(0)
  bkyber.io.rst_n := !reset.asBool()

  // Connect control signals
  bkyber.io.enable := io.enable
  bkyber.io.key_enable := io.key_enable
  bkyber.io.encryption_enable := io.encryption_enable
  bkyber.io.decryption_enable := io.decryption_enable

  // Initialize request signals
  bkyber.io.data_Req := DontCare
  bkyber.io.addr_Req := DontCare
  bkyber.io.wen_Req := DontCare
  bkyber.io.bytelane_Req := DontCare

  // *** INTERRUPT GENERATION (Timer style) ***
  // Convert done signals to interrupts
  io.cio_babykyber_intr_key := bkyber.io.key_done
  io.cio_babykyber_intr_encrypt := bkyber.io.encryption_done
  io.cio_babykyber_intr_decrypt := bkyber.io.decryption_done

  dontTouch(io.req.valid)

  // Bus interface logic (same as before)
  when(io.req.valid && io.req.bits.isWrite) {
    // Write operation
    bkyber.io.data_Req := io.req.bits.dataRequest.asSInt
    bkyber.io.addr_Req := io.req.bits.addrRequest.asSInt
    bkyber.io.wen_Req := io.req.bits.isWrite
    bkyber.io.bytelane_Req := io.req.bits.activeByteLane
    rdata := bkyber.io.data_Resp
    validReg := true.B
  }.elsewhen(io.req.valid && !io.req.bits.isWrite) {
    // Read operation
    validReg := true.B
    bkyber.io.wen_Req := false.B
    bkyber.io.addr_Req := io.req.bits.addrRequest.asSInt
    rdata := bkyber.io.data_Resp
  }.otherwise {
    validReg := false.B
    rdata := DontCare
  }

  io.rsp.bits.dataResponse := rdata.asUInt
  io.rsp.bits.error := false.B  // No error handling for now
}

// *** BLACKBOX DEFINITIONS - UNCHANGED ***
class BabyKyberIO extends Bundle {
  val clk = Input(Bool())
  val rst_n = Input(Bool())
  val enable = Input(Bool())
  val key_enable = Input(Bool())
  val encryption_enable = Input(Bool())
  val decryption_enable = Input(Bool())
  val data_Req = Input(SInt(32.W))
  val addr_Req = Input(SInt(32.W))
  val wen_Req = Input(Bool())
  val bytelane_Req = Input(UInt(8.W))
  val data_Resp = Output(SInt(32.W))
  val key_done = Output(Bool())
  val encryption_done = Output(Bool()) 
  val decryption_done = Output(Bool()) 
}

class kybertop extends BlackBox with HasBlackBoxResource {
  val io = IO(new BabyKyberIO)
  addResource("/Baby_Kyber/kybertop.sv")
  addResource("/Baby_Kyber/Decrypt.sv")
  addResource("/Baby_Kyber/Encrypt.sv")
  addResource("/Baby_Kyber/KeyGeneration.sv")
  addResource("/Baby_Kyber/PolynomialMatrixMultiplication.sv")
  addResource("/Baby_Kyber/MatrixTranspose.sv")
}