let s:cpo_save=&cpo
set cpo&vim
cnoremap <silent> <Plug>(TelescopeFuzzyCommandSearch) e "lua require('telescope.builtin').command_history { default_text = [=[" . escape(getcmdline(), '"') . "]=] }"
inoremap <C-W> u
inoremap <C-U> u
vnoremap 	 >gv^
nnoremap 	 	zz
nnoremap  zz
nnoremap  <Cmd>copenzz
nmap  d
nnoremap  <Cmd>noh
nnoremap  ft <Cmd>TodoTelescope
nnoremap  fc :Telescope neoclip
vnoremap  k <Cmd>WhichKey
nnoremap  k <Cmd>WhichKey
nnoremap  oT <Cmd>ObsidianTags
nnoremap  ol <Cmd>ObsidianLinks
nnoremap  or <Cmd>ObsidianRename
nnoremap  on :ObsidianNew 
nnoremap  ot <Cmd>ObsidianTemplate
nnoremap  s :%s/\<\>//gIc<Left><Left><Left><Left>
nnoremap  dd "_dd
nnoremap  d "_d
vnoremap  d "_d
nnoremap  yy "+yy
nnoremap  y "+y
vnoremap  y "+y
nnoremap  P "+P
nnoremap  p "+p
xnoremap  p "_d"+P
nnoremap  / /+
nnoremap  X :%!
nnoremap  x :!
nnoremap  w <Cmd>w
nnoremap  q q
nnoremap  - s
nnoremap  \ v
omap <silent> % <Plug>(MatchitOperationForward)
xmap <silent> % <Plug>(MatchitVisualForward)
nmap <silent> % <Plug>(MatchitNormalForward)
nnoremap & :&&
nnoremap + 
nnoremap - 
vnoremap / "fy/f
xnoremap <silent> <expr> @ mode() ==# 'V' ? ':normal! @'.getcharstr().'' : '@'
nnoremap J mzJ'z
vnoremap J :m '>+1gv=gv
vnoremap K :m '<-2gv=gv
nnoremap N Nzz
nnoremap Q <Nop>
xnoremap <silent> <expr> Q mode() ==# 'V' ? ':normal! @=reg_recorded()' : 'Q'
nnoremap Y y$
omap <silent> [% <Plug>(MatchitOperationMultiBackward)
xmap <silent> [% <Plug>(MatchitVisualMultiBackward)
nmap <silent> [% <Plug>(MatchitNormalMultiBackward)
nnoremap [q <Cmd>cprevzz
omap <silent> ]% <Plug>(MatchitOperationMultiForward)
xmap <silent> ]% <Plug>(MatchitVisualMultiForward)
nmap <silent> ]% <Plug>(MatchitNormalMultiForward)
nnoremap ]q <Cmd>cnextzz
xmap a% <Plug>(MatchitVisualTextObject)
omap <silent> g% <Plug>(MatchitOperationBackward)
xmap <silent> g% <Plug>(MatchitVisualBackward)
nmap <silent> g% <Plug>(MatchitNormalBackward)
xnoremap <silent> gs <Cmd>lua MiniOperators.sort('visual')
nmap <silent> gss ^gsg_
nnoremap <silent> <expr> gs v:lua.MiniOperators.sort()
xnoremap <silent> gr <Cmd>lua MiniOperators.replace('visual')
nmap <silent> grr gr_
nnoremap <silent> <expr> gr v:lua.MiniOperators.replace()
xnoremap <silent> gm <Cmd>lua MiniOperators.multiply('visual')
nmap <silent> gmm gm_
nnoremap <silent> <expr> gm v:lua.MiniOperators.multiply()
nmap <silent> gxx gx_
xnoremap <silent> g= <Cmd>lua MiniOperators.evaluate('visual')
nmap <silent> g== g=_
nnoremap <silent> <expr> g= v:lua.MiniOperators.evaluate()
xnoremap <silent> gx <Cmd>lua MiniOperators.exchange('visual')
nnoremap <silent> <expr> gx v:lua.MiniOperators.exchange()
nnoremap n nzz
xnoremap p "_dP
nnoremap yc yy<Cmd>normal gccp
vnoremap za zA
nnoremap za zA
vnoremap zo zR
nnoremap zo zR
vnoremap zc zM
nnoremap zc zM
nnoremap <SNR>31_: :=v:count ? v:count : ''
xmap <silent> <Plug>(MatchitVisualTextObject) <Plug>(MatchitVisualMultiBackward)o<Plug>(MatchitVisualMultiForward)
onoremap <silent> <Plug>(MatchitOperationMultiForward) :call matchit#MultiMatch("W",  "o")
onoremap <silent> <Plug>(MatchitOperationMultiBackward) :call matchit#MultiMatch("bW", "o")
xnoremap <silent> <Plug>(MatchitVisualMultiForward) :call matchit#MultiMatch("W",  "n")m'gv``
xnoremap <silent> <Plug>(MatchitVisualMultiBackward) :call matchit#MultiMatch("bW", "n")m'gv``
nnoremap <silent> <Plug>(MatchitNormalMultiForward) :call matchit#MultiMatch("W",  "n")
nnoremap <silent> <Plug>(MatchitNormalMultiBackward) :call matchit#MultiMatch("bW", "n")
onoremap <silent> <Plug>(MatchitOperationBackward) :call matchit#Match_wrapper('',0,'o')
onoremap <silent> <Plug>(MatchitOperationForward) :call matchit#Match_wrapper('',1,'o')
xnoremap <silent> <Plug>(MatchitVisualBackward) :call matchit#Match_wrapper('',0,'v')m'gv``
xnoremap <silent> <Plug>(MatchitVisualForward) :call matchit#Match_wrapper('',1,'v'):if col("''") != col("$") | exe ":normal! m'" | endifgv``
nnoremap <silent> <Plug>(MatchitNormalBackward) :call matchit#Match_wrapper('',0,'n')
nnoremap <silent> <Plug>(MatchitNormalForward) :call matchit#Match_wrapper('',1,'n')
nnoremap <Plug>PlenaryTestFile :lua require('plenary.test_harness').test_file(vim.fn.expand("%:p"))
vnoremap <S-Tab> <gv^
nnoremap <C-Q> <Cmd>copenzz
nnoremap <C-I> 	zz
nnoremap <C-O> zz
nmap <C-W><C-D> d
inoremap <expr>  v:lua.require'nvim-autopairs'.completion_confirm()
inoremap  u
inoremap  u
let &cpo=s:cpo_save
unlet s:cpo_save
set nofixendofline
set foldlevelstart=99
set grepformat=%f:%l:%c:%m
set grepprg=rg\ --vimgrep\ -uu\ 
set helplang=en
set history=100
set ignorecase
set noincsearch
set isfname=#,$,%,+,,,-,.,/,48-57,=,@,_,~,@-@
set laststatus=3
set noloadplugins
set mouse=
set operatorfunc=v:lua.require'vim._comment'.operator
set packpath=/usr/share/nvim/runtime
set path=.,,/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/usr/include/
set pumheight=10
set runtimepath=~/.config/nvim,~/.local/share/nvim/lazy/lazy.nvim,~/.local/share/nvim/lazy/image.nvim,~/.local/share/nvim/lazy/render-markdown.nvim,~/.local/share/nvim/lazy/LuaSnip,~/.local/share/nvim/lazy/neogen,~/.local/share/nvim/lazy/obsidian.nvim,~/.local/share/nvim/lazy/nvim-ts-autotag,~/.local/share/nvim/lazy/nvim-treesitter,~/.local/share/nvim/lazy/nvim-autopairs,~/.local/share/nvim/lazy/lspkind.nvim,~/.local/share/nvim/lazy/nvim-cmp,~/.local/share/nvim/lazy/cmp-path,~/.local/share/nvim/lazy/cmp-nvim-lsp,~/.local/share/nvim/lazy/mason-lspconfig.nvim,~/.local/share/nvim/lazy/mason.nvim,~/.local/share/nvim/lazy/nvim-lspconfig,~/.local/share/nvim/lazy/indent-blankline.nvim,~/.local/share/nvim/lazy/nvim-treesitter-context,~/.local/share/nvim/lazy/nvim-web-devicons,~/.local/share/nvim/lazy/which-key.nvim,~/.local/share/nvim/lazy/lsp_signature.nvim,~/.local/share/nvim/lazy/hex.nvim,~/.local/share/nvim/lazy/snacks.nvim,~/.local/share/nvim/lazy/mini.operators,~/.local/share/nvim/lazy/sqlite.lua,~/.local/share/nvim/lazy/nvim-neoclip.lua,~/.local/share/nvim/lazy/gitsigns.nvim,~/.local/share/nvim/lazy/harpoon,~/local/elements,~/.local/share/nvim/lazy/telescope-fzf-native.nvim,~/.local/share/nvim/lazy/popup.nvim,~/.local/share/nvim/lazy/telescope-zoxide,~/.local/share/nvim/lazy/todo-comments.nvim,~/.local/share/nvim/lazy/ripgrep,~/.local/share/nvim/lazy/fd,~/.local/share/nvim/lazy/plenary.nvim,~/.local/share/nvim/lazy/telescope.nvim,~/.local/share/nvim/lazy/vim-fugitive,~/local/undotree,~/.local/share/nvim/lazy/tint.nvim,~/.local/share/nvim/lazy/tokyonight.nvim,/usr/share/nvim/runtime,/usr/share/nvim/runtime/pack/dist/opt/matchit,/usr/lib64/nvim,~/.local/state/nvim/lazy/readme,~/.local/share/nvim/lazy/cmp-path/after,~/.local/share/nvim/lazy/cmp-nvim-lsp/after,~/.local/share/nvim/lazy/indent-blankline.nvim/after
set scrolloff=8
set shell=/bin/zsh
set showtabline=2
set smartcase
set smartindent
set softtabstop=8
set splitbelow
set splitright
set statusline=%!v:lua.require('elements.statusline')
set noswapfile
set tabline=%!v:lua.require('elements.tabline').harpoon_files()
set termguicolors
set undodir=~/.config/nvim/undo
set undofile
set updatetime=50
set winbar=%!v:lua.require('elements.winbar')
set window=70
" vim: set ft=vim :
