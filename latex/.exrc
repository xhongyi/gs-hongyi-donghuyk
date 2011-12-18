if &cp | set nocp | endif
nmap ,s :source $MYVIMRC
nmap ,e :e $MYVIMRC
nmap Y y$
let s:cpo_save=&cpo
set cpo&vim
nmap gx <Plug>NetrwBrowseX
nmap j gj
nmap k gk
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
let &cpo=s:cpo_save
unlet s:cpo_save
set autoread
set backspace=2
set expandtab
set fileencodings=ucs-bom,utf-8,default,latin1
set grepprg=grep\ -nH\ $*
set helplang=en
set history=50
set hlsearch
set nomodeline
set path=.,./inc,./include
set printoptions=paper:letter
set ruler
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim73,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set shiftwidth=4
set smartindent
set smarttab
set softtabstop=4
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set tabstop=4
set tags=./tags,../tags,../../tags,../../../tags
set textwidth=80
set visualbell
set whichwrap=b,s,<,>,h,l
" vim: set ft=vim :
