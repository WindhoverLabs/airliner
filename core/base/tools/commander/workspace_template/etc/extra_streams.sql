--Configuration tested on YAMCS 5.4.0
--TODO: This should be auto-generated and driven by configuration. That way we can even abstract the protocol(CCSDS, MAVLink, etc) if need be.
create stream cfdp_in as select substring(packet, 12) as pdu from tm_realtime where extract_short(packet, 0) = 2622
create stream cfdp_out (gentime TIMESTAMP, entityId long, seqNum int, pdu  binary)
insert into tc_realtime select gentime, 'cfdp-service' as origin, seqNum, '/yamcs/cfdp/upload' as cmdName, unhex('1A3AC00000000000') + pdu as binary from cfdp_out
