<?xml version="1.0" encoding="UTF-8"?>
<simconf version="2023090101">
  <simulation>
    <title>AODV MANET 10 nodes</title>
    <randomseed>12345</randomseed>
    <motedelay_us>1000000</motedelay_us>
    <radiomedium>
      org.contikios.cooja.radiomediums.UDGM
      <transmitting_range>50.0</transmitting_range>
      <interference_range>100.0</interference_range>
      <success_ratio_tx>1.0</success_ratio_tx>
      <success_ratio_rx>1.0</success_ratio_rx>
    </radiomedium>
    <events>
      <event>
        <time>3600000</time>
        <command>quit</command>
      </event>
    </events>
    <motetype>
      org.contikios.cooja.contikimote.ContikiMoteType
      <identifier>aodv-demo</identifier>
      <description>AODV/UDP Flooding Mote</description>
      <source>[CONFIG_DIR]/../src/aodv-demo.c</source>
      <commands>$(MAKE) -j$(CPUS) aodv-demo.cooja TARGET=cooja -f Makefile.aodv</commands>
      <firmware>[CONFIG_DIR]/../src/build/cooja/aodv-demo.cooja</firmware>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="20" y="20" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="40" y="40" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="60" y="60" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="80" y="80" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="100" y="100" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="120" y="120" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="140" y="140" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="160" y="160" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="180" y="180" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="200" y="200" />
        </interface_config>
      </mote>
    </motetype>
  </simulation>
  <plugin>
    org.contikios.cooja.plugins.ScriptRunner
    <plugin_config>
      <script>
        while (true) {
          YIELD();
          if (typeof msg !== 'undefined' &amp;&amp; msg !== null) {
            log.log(String(msg) + "\n");
          }
        }
      </script>
      <active>true</active>
    </plugin_config>
  </plugin>
</simconf> 