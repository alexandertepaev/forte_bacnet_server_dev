<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE AdapterType SYSTEM "http://www.holobloc.com/xml/LibraryElement.dtd">
<AdapterType Comment="Adapter Interface" Name="BACnetAdapter">
  <Identification Standard="61499-1"/>
  <VersionInfo Author="root" Date="2019-10-24" Version="1.0"/>
  <CompilerInfo header="package fb.test;"/>
  <InterfaceList>
    <EventInputs>
      <Event Comment="Request from Socket" Name="INITO" Type="Event"/>
    </EventInputs>
    <EventOutputs>
      <Event Comment="Confirmation from Plug" Name="INIT" Type="Event"/>
    </EventOutputs>
  </InterfaceList>
</AdapterType>
