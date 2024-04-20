/**
 * @author Wei
 * NetworkTable component for displaying nearby networks.
 */

"use client";

import {
  Table,
  TableBody,
  TableCaption,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from "@/components/ui/table";

import { Badge } from "@/components/ui/badge";
import { AUTH_MODE_NAMES, NetworkItem } from "@/lib/types";
import { Skeleton } from "./ui/skeleton";
import { ConnectDialog } from "./connect-dialog";

export function NetworkTable({ networks }: { networks: NetworkItem[] }) {
  return (
    <Table>
      <TableCaption>List of all visible networks.</TableCaption>
      <TableHeader>
        <TableRow>
          <TableHead className="w-[100px]">SSID</TableHead>
          <TableHead>Auth</TableHead>
          <TableHead>RSSI</TableHead>
          <TableHead className="text-right">Channel</TableHead>
          {/* <TableHead className="text-right">BSSID</TableHead> */}
        </TableRow>
      </TableHeader>
      <TableBody className="max-h-[300px] overflow-y-auto">
        {networks.map((network) => {
          const [_ssid, rssi, auth, channel, bssid] = network;

          return (
            <TableRow key={bssid}>
              <TableCell>
                <ConnectDialog network={network} />
              </TableCell>
              <TableCell>
                <Badge>{AUTH_MODE_NAMES[auth]}</Badge>
              </TableCell>
              <TableCell>{rssi}</TableCell>
              <TableCell className="text-right">{channel}</TableCell>
              {/* <TableCell className="text-right font-mono">{bssid}</TableCell> */}
            </TableRow>
          );
        })}
      </TableBody>
    </Table>
  );
}

export function NetworkTableSkeleton() {
  return (
    <div className="flex flex-col w-full gap-2">
      {Array.from({ length: 5 }).map((_, i) => (
        <Skeleton className="h-16 w-full" key={i} />
      ))}
    </div>
  );
}
