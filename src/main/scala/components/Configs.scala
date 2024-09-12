package nucleusrv.components

case class Configs(
    XLEN    : Int     = 32,
    M       : Boolean = true,
    C       : Boolean = true,
    Zicsr   : Boolean = true,
    TRACE   : Boolean = true
)