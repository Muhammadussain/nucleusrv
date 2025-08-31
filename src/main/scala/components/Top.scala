package nucleusrv.components
import chisel3._
import chisel3.stage.ChiselStage
import nucleusrv.tracer._
import BabyKyber.BabyKyberHarness

class Top(programFile:Option[String], dataFile:Option[String]) extends Module{

  val io = IO(new Bundle() {
    val pin = Output(UInt(32.W))
    val rvfi = new TracerO
  })

  implicit val config: Configs = Configs(
    XLEN = 32,
    M = true,
    F = true,
    C = false,
    Zicsr = true,
    TRACE = true
  )

  // pass explicit WishboneConfig to harness (fully-qualified to avoid import)
 val babyKyberHarness = Module(new BabyKyberHarness())

  val core: Core = Module(new Core())
  core.io.stall := false.B


  // Enable is always on, triggers are from core
  babyKyberHarness.io.enable := true.B
  babyKyberHarness.io.key_enable := core.io.key_enable_trigger
  babyKyberHarness.io.encryption_enable := core.io.encryption_enable_trigger
  babyKyberHarness.io.decryption_enable := core.io.decryption_enable_trigger
  // Optionally, connect or monitor done signals
  // val keyDone = babyKyber.io.key_done
  // val encryptionDone = babyKyber.io.encryption_done
  // val decryptionDone = babyKyber.io.decryption_done

  core.io.baby_kyber match {
    case Some(bkIO) =>
      // Core <-> Harness
      bkIO.rsp.bits.error := false.B
      babyKyberHarness.io.req <> bkIO.req
      bkIO.rsp <> babyKyberHarness.io.rsp
      // Interrupts (core ko wapas)
      bkIO.cio_babykyber_intr_key := babyKyberHarness.io.cio_babykyber_intr_key
      bkIO.cio_babykyber_intr_encrypt := babyKyberHarness.io.cio_babykyber_intr_encrypt
      bkIO.cio_babykyber_intr_decrypt := babyKyberHarness.io.cio_babykyber_intr_decrypt
    case None => // Not enabled
  }

  
  

  val dmem = Module(new SRamTop(dataFile))
  val imem = Module(new SRamTop(programFile))

  /*  Imem Interceonnections  */
  core.io.imemRsp <> imem.io.rsp
  imem.io.req <> core.io.imemReq


  /*  Dmem Interconnections  */
  core.io.dmemRsp <> dmem.io.rsp
  dmem.io.req <> core.io.dmemReq

  io.pin := core.io.pin

  if (config.TRACE) {
    val tracer = Module(new Tracer)
    tracer.rvfi_i <> core.io.rvfi.get
    io.rvfi <> tracer.rvfi_o
  }
}

object NRVDriver {
  // generate verilog
  def main(args: Array[String]): Unit = {
      val IMem = if (args.contains("--imem")) Some(args(args.indexOf("--imem") + 1)) else None
      val DMem = if (args.contains("--dmem")) Some(args(args.indexOf("--dmem") + 1)) else None
      new ChiselStage().emitVerilog(
        new Top(IMem, DMem),
        if (args.contains("--target-dir")) args.slice(
          args.indexOf("--target-dir"),
          args.indexOf("--target-dir") + 2
        ) else Array()
      )
  }
}
