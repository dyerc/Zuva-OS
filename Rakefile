task :default => [:run]

task :build do
  Dir.chdir('src/kernel'){
    %x[make]
  }
end

task :iso do
  %x[mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o zuva.iso root]
end

task :clean do
  Dir.chdir('src/kernel'){
    %x[make clean]
  }
end

task :run => [:build, :iso] do
  %x[qemu-system-i386 -cdrom zuva.iso]
end
