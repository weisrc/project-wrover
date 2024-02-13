"use client";

import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";

export function InputField({
  label,
  value,
  type,
  onValueChange,
}: {
  label: string;
  value: string;
  type?: string;
  onValueChange: (value: string) => void;
}) {
  return (
    <div className="grid grid-cols-4 items-center gap-4 my-2">
      <Label className="text-right">{label}</Label>
      <Input
        value={value}
        type={type}
        onChange={(e) => onValueChange(e.target.value)}
        className="col-span-3"
      />
    </div>
  );
}
