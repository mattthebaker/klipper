.section .rodata.bootstrap_payload


.global _bootstrap_payload_start
.weak _bootstrap_payload_start

.global _bootstrap_payload_end
.weak _bootstrap_payload_end

.global _bootstrap_payload_size
.weak _bootstrap_payload_size

.align 4
_bootstrap_payload_start:

.incbin "src/stm32/bootstrap_payload.bin"

.align 4
_bootstrap_payload_end:

.set _bootstrap_payload_size, _bootstrap_payload_end


