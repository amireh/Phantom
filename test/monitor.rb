#!/usr/bin/env ruby


system("kill -s 10 `pgrep Server`")
sleep(1)
f = File.open("../dumps/data/stats.out", File::RDONLY)
begin
  puts "+= Elementum Server Stats +="
  uptime = f.readline
  nowtime = f.readline
  conns = { :total => f.readline.gsub("\n", ""), :active => f.readline.gsub("\n", "") }
  instances = { :total => f.readline.gsub("\n", ""), :active => f.readline.gsub("\n", "") }

  puts "Uptime: #{uptime}"
  puts "Reported at: #{nowtime}"
  puts ""
  puts "# connections: #{conns[:total]} (#{conns[:active]} active)"
  puts "# instances: #{instances[:total]} (#{instances[:active]} active)"
  puts ""
  puts "+= Elementum Server Stats +="
rescue
  puts "couldn't read file"
end
