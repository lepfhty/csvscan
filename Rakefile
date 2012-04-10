# -*- ruby -*-

require 'hoe'

module CSVScan
  VERSION = "0.1.1"
end

Hoe.plugin :compiler

Hoe.spec 'csvscan' do
  developer('Ben Sandofsky', 'sandofsky@gmail.com')
  developer('Ricky Nguyen', 'ricky.nguyen@gmail.com')
  spec_extras[:extensions] = "ext/csvscan/extconf.rb"

  ragel_cout = 'ext/csvscan/csvscan.c'
  task :compile => ragel_cout

  if (system "ragel --version > /dev/null")
    file ragel_cout => ['ext/csvscan/csvscan.rl'] do |f|
      s = "ragel -G2 #{f.prerequisites.join} -o #{f.name}"
      puts s
      system s
    end
    clean_globs << ragel_cout
  end

end

# vim: syntax=ruby
