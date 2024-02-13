"use client"

import { RemoteConnectCard } from "@/components/remote-connect-card"
import { connectWebSocket } from "@/lib/connect-web-socket"
import { useRouter, useSearchParams } from "next/navigation"
import { useEffect, useState } from "react"

export default function Remote() {
    const [connected, setConnected] = useState(false)

    return <div className="flex flex-col items-center h-screen pt-16">
        {connected ? (
            "You are connected"
        ) : (
            <RemoteConnectCard onConnectionChange={setConnected} />
        )}
    </div>



}