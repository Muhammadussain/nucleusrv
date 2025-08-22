package BabyKyber
// import caravan.bus.tilelink.{TLRequest, TLResponse, TilelinkConfig, TilelinkDevice, TilelinkHost}
import caravan.bus.wishbone.{WBRequest, WBResponse, WishboneConfig, WishboneDevice, WishboneHost}
import chisel3._
import chisel3.stage.ChiselStage
import chisel3.util.Decoupled

class BabyKyberHarness(implicit val config: WishboneConfig ) extends Module {
  val io = IO(new Bundle {

    // bus interconnect interfaces
    val req = Flipped(Decoupled(new WBRequest()))
    val rsp = Decoupled(new WBResponse())

    // *** EXTERNAL CONTROL SIGNALS (testbench se control) ***
    val enable = Input(Bool())
    val key_enable = Input(Bool())
    val encryption_enable = Input(Bool())
    val decryption_enable = Input(Bool())

    // *** INTERRUPT OUTPUTS ***
    val cio_babykyber_intr_key = Output(Bool())
    val cio_babykyber_intr_encrypt = Output(Bool())
    val cio_babykyber_intr_decrypt = Output(Bool())
  })
  
  val hostAdapter = Module(new WishboneHost())
  val deviceAdapter = Module(new WishboneDevice())
  val babykyber = Module(new BabyKyberTop(new WBRequest(), new WBResponse()))

  hostAdapter.io.reqIn <> io.req
  io.rsp <> hostAdapter.io.rspOut
  hostAdapter.io.wbMasterTransmitter <> deviceAdapter.io.wbMasterReceiver
  hostAdapter.io.wbSlaveReceiver <> deviceAdapter.io.wbSlaveTransmitter

  babykyber.io.req <> deviceAdapter.io.reqOut
  babykyber.io.rsp <> deviceAdapter.io.rspIn

  // *** CONTROL SIGNALS - TESTBENCH SE AAYENGE ***
  babykyber.io.enable := io.enable
  babykyber.io.key_enable := io.key_enable
  babykyber.io.encryption_enable := io.encryption_enable
  babykyber.io.decryption_enable := io.decryption_enable

  // *** INTERRUPT CONNECTIONS ***
  io.cio_babykyber_intr_key := babykyber.io.cio_babykyber_intr_key
  io.cio_babykyber_intr_encrypt := babykyber.io.cio_babykyber_intr_encrypt
  io.cio_babykyber_intr_decrypt := babykyber.io.cio_babykyber_intr_decrypt
}

object BabyKyberDriverWB extends App {
  implicit val config = WishboneConfig(32,32)
  (new ChiselStage).emitVerilog(new BabyKyberHarness())
}

/*
// *** TILELINK CODE COMMENTED OUT ***

class BabyKyberHarnessTL(implicit val config: TilelinkConfig ) extends Module {
  val io = IO(new Bundle {

    // bus interconnect interfaces
    val req = Flipped(Decoupled(new TLRequest()))
    val rsp = Decoupled(new TLResponse())

    // *** EXTERNAL CONTROL SIGNALS (testbench se control) ***
    val enable = Input(Bool())
    val key_enable = Input(Bool())
    val encryption_enable = Input(Bool())
    val decryption_enable = Input(Bool())

    val cio_babykyber_intr_key = Output(Bool())
    val cio_babykyber_intr_encrypt = Output(Bool())
    val cio_babykyber_intr_decrypt = Output(Bool())
    
  })
  val hostAdapter = Module(new TilelinkHost())
  val deviceAdapter = Module(new TilelinkDevice())
  val babykyber = Module(new BabyKyberTop(new TLRequest(), new TLResponse()))

  hostAdapter.io.reqIn <> io.req
  io.rsp <> hostAdapter.io.rspOut
  hostAdapter.io.tlMasterTransmitter <> deviceAdapter.io.tlMasterReceiver
  hostAdapter.io.tlSlaveReceiver <> deviceAdapter.io.tlSlaveTransmitter

  babykyber.io.req <> deviceAdapter.io.reqOut
  babykyber.io.rsp <> deviceAdapter.io.rspIn

  // *** CONTROL SIGNALS - TESTBENCH SE AAYENGE ***
  babykyber.io.enable := io.enable
  babykyber.io.key_enable := io.key_enable
  babykyber.io.encryption_enable := io.encryption_enable
  babykyber.io.decryption_enable := io.decryption_enable

  io.cio_babykyber_intr_key := babykyber.io.cio_babykyber_intr_key
  io.cio_babykyber_intr_encrypt := babykyber.io.cio_babykyber_intr_encrypt
  io.cio_babykyber_intr_decrypt := babykyber.io.cio_babykyber_intr_decrypt

}

object BabyKyberDriverTL extends App {
  implicit val config = TilelinkConfig()
  (new ChiselStage).emitVerilog(new BabyKyberHarnessTL())
}
*/