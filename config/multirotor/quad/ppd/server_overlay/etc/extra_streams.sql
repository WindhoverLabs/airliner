--Configuration tested on YAMCS 5.4.0
--TODO: This should be auto-generated and driven by configuration. That way we can even abstract the protocol(CCSDS, MAVLink, etc) if need be.
create stream cfdp_ch1_in as select substring(packet, 12) as pdu from tm_realtime where extract_short(packet, 0) = 2081
create stream cfdp_ch2_in as select substring(packet, 12) as pdu from tm_realtime where extract_short(packet, 0) = 2082
create stream cfdp_ch1_out (gentime TIMESTAMP, entityId long, seqNum int, pdu  binary)
insert into tc_realtime select gentime, 'cfdp-service' as origin, seqNum, '/yamcs/cfdp/upload' as cmdName, unhex('186FC00000000000') + pdu as binary from cfdp_ch1_out
create stream cfdp_ch2_out (gentime TIMESTAMP, entityId long, seqNum int, pdu  binary)
insert into tc_realtime select gentime, 'cfdp-service' as origin, seqNum, '/yamcs/cfdp/upload' as cmdName, unhex('1870C00000000000') + pdu as binary from cfdp_ch2_out
