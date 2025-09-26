package nucleusrv.components
import chisel3._
import chisel3.stage.ChiselStage
import chisel3.util.Decoupled
import nucleusrv.tracer._

// Local bundle definitions to avoid caravan imports
class LocalWBRequest extends Bundle {
  val addrRequest = UInt(32.W)
  val dataRequest = UInt(32.W)
  val activeByteLane = UInt(4.W)
  val isWrite = Bool()
}

class LocalWBResponse extends Bundle {
  val dataResponse = UInt(32.W)
  val error = Bool()
}

class Top(programFile:Option[String], dataFile:Option[String]) extends Module{

  val io = IO(new Bundle() {
    val pin = Output(UInt(32.W))
    val rvfi = new TracerO
    // Expose baby_kyber interface with local bundles
    val baby_kyber = new Bundle {
      val req = Decoupled(new LocalWBRequest())
      val rsp = Flipped(Decoupled(new LocalWBResponse()))
      val cio_babykyber_intr_key = Input(Bool())
      val cio_babykyber_intr_encrypt = Input(Bool())
      val cio_babykyber_intr_decrypt = Input(Bool())
    }
    // Expose trigger signals
    val key_enable_trigger = Output(Bool())
    val encryption_enable_trigger = Output(Bool())
    val decryption_enable_trigger = Output(Bool())
  })

  implicit val config: Configs = Configs(
    XLEN = 32,
    M = true,
    F = true,
    C = false,
    Zicsr = true,
    TRACE = true
  )

  val core: Core = Module(new Core())
  core.io.stall := false.B

  // Connect trigger signals
  io.key_enable_trigger := core.io.key_enable_trigger
  io.encryption_enable_trigger := core.io.encryption_enable_trigger
  io.decryption_enable_trigger := core.io.decryption_enable_trigger

  core.io.baby_kyber match {
    case Some(bkIO) =>
      // Connect core to external IO
      io.baby_kyber.req.valid := bkIO.req.valid
      io.baby_kyber.req.bits := bkIO.req.bits
      bkIO.req.ready := io.baby_kyber.req.ready

      bkIO.rsp.valid := io.baby_kyber.rsp.valid
      bkIO.rsp.bits := io.baby_kyber.rsp.bits
      io.baby_kyber.rsp.ready := bkIO.rsp.ready

      bkIO.cio_babykyber_intr_key := io.baby_kyber.cio_babykyber_intr_key
      bkIO.cio_babykyber_intr_encrypt := io.baby_kyber.cio_babykyber_intr_encrypt
      bkIO.cio_babykyber_intr_decrypt := io.baby_kyber.cio_babykyber_intr_decrypt
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
