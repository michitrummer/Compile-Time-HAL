# MultiCall

MultiCall uses nine distinct LED/switch module IDs rather than repeatedly
accessing the same GPIO pair.
`ld4`/`b1` represent the actual Nucleo modules; `ex1` … `ex8` are
benchmark-only BSL mappings used to diversify port addresses, pin masks and
GPIO configurations.
