/**
 * @author Wei
 * HiddenNetworkSetup component
 */

import { AuthMode } from "@/lib/types";
import { useState } from "react";
import { InputField } from "./input-field";
import { NetworkConnectionForm } from "./network-connection-form";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "./ui/select";

export function HiddenNetworkSetup() {
  const [auth, setAuth] = useState<AuthMode>("wpa2");
  const [ssid, setSsid] = useState("");

  return (
    <div>
      <Select
        value={auth}
        onValueChange={(value) => setAuth(value as AuthMode)}
      >
        <SelectTrigger className="w-[180px]">
          <SelectValue />
        </SelectTrigger>
        <SelectContent>
          <SelectItem value="wpa2">WPA2</SelectItem>
          <SelectItem value="wpa2_enterprise">WPA2 Enterprise</SelectItem>
          <SelectItem value="open">Open</SelectItem>
        </SelectContent>
      </Select>

      <InputField label="SSID" value={ssid} onValueChange={setSsid} />

      <NetworkConnectionForm ssid={ssid} auth={auth} />
    </div>
  );
}
