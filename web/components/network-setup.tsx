/**
 * @author Wei
 * Network setup component.
 */

"use client";

import {
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
} from "@/components/ui/card";

import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { NAME } from "@/lib/common";
import { VisibleNetworkSetup } from "./visible-network-setup";
import { HiddenNetworkSetup } from "./hidden-network-setup";

export function NetworkSetup() {
  return (
    <Card className="border-none shadow-none">
      <CardHeader>
        <CardTitle>{NAME} WiFi Setup</CardTitle>
        <CardDescription>
          Connect the {NAME} to wireless network.
        </CardDescription>
      </CardHeader>
      <CardContent>
        <Tabs defaultValue="visible">
          <TabsList>
            <TabsTrigger value="visible">Visible Network</TabsTrigger>
            <TabsTrigger value="hidden">Hidden Network</TabsTrigger>
          </TabsList>
          <TabsContent value="visible">
            <VisibleNetworkSetup />
          </TabsContent>
          <TabsContent value="hidden">
            <HiddenNetworkSetup />
          </TabsContent>
        </Tabs>
      </CardContent>
    </Card>
  );
}
